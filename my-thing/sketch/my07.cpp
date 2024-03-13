
#include "Thing.h"

// the replacement_t type definition allows specification of a subset of the
// "boilerplate" strings, so we can e.g. replace only the title, or etc.
//
// note that to see the results you need to load a page at "/7", e.g. 
// http://192.168.4.1/7

const char *htmltemplates[]={
    "<html><head><title>",                                                // 0
  "default title (7)",                                                  // 1
  "</title>\n",                                                         // 2
  "<meta charset='utf-8'>",                                             // 3

  // adjacent strings in C are concatenated:
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
  "<style>body{background:#FFF; color: #000; font-family: sans-serif;", // 4

  "font-size: 150%;}</style>\n",                                        // 5
  "</head><body>\n<h2>",                                                // 6
  "Welcome to Thing (again)!",                                          // 7
  "</h2>\n<p><a href='/'>Home</a>&nbsp;&nbsp;&nbsp;</p>\n",             // 8
  "</body></html>\n\n",                                                 // 9
};


void mysetup07() {
  setup06(); dln(startupDBG, "\nmysetup07...");
  blink(3, 500);
  webServer.on("/7", myHandleSeven);
}

void myloop07() {
    if(! (loopIteration++ % 500000 == 0)) // a slice every 0.5m iterations
        return;
    dln(loopDBG, "\nI am here....");
    webServer.handleClient(); // serve pending web requests every loop
}

void getMyHtml( // turn array of strings & set of replacements into a String
  String& html, const char *htmltemplates[], int boilerLen,
  replacement_t repls[], int replsLen
) {
  for(int i = 0, j = 0; i < boilerLen; i++) {
    if(j < replsLen && repls[j].position == i)
      html.concat(repls[j++].replacement);
    else
      html.concat(htmltemplates[i]);
  }
}


void myHandleSeven() {
  dln(netDBG, "handleSeven: serving page at /7");
  replacement_t repls[] = { // the elements to replace in the boilerplate
    { 1, "The best title" },
    { 7, "Eat more steaks!" },
  };
  String toSend = "";
  getMyHtml(toSend, htmltemplates, MYALEN(htmltemplates), repls, MYALEN(repls)); // instantiate
  webServer.send(200, "text/html", toSend);
}