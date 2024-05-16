# Simple flask server to hanlde /sos post requests and get /version.txt
import os
import json
from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/sos', methods=['POST'])
def sos():
    data = request.get_json()
    print("SOS message recieved: ",data)
    return jsonify({'status': 'ok'})

@app.route('/version.txt', methods=['GET'])
def version():
    print("Request for version.txt")
    with open('version.txt', 'r') as f:
        return f.read()
    
# root for [number].bin files
@app.route('/<path:path>', methods=['GET'])
def root(path):
    print("Request for: ",path)
    with open(path, 'rb') as f:
        return f.read()
# else return 404
@app.errorhandler(404)
def page_not_found(e):
    print("404 Not Found")
    return "404 Not Found", 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)