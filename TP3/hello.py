import json
from flask import jsonify
from flask import Flask
from flask import render_template
from flask import abort
from flask import request
app = Flask(__name__)

welcome = {"sentence" : "Welcome to 3ESE API!"}

@app.route('/')
def hello_world():
    return 'Hello, World!\n'

@app.route('/api/welcome/', methods=['GET', 'POST'])
def api_welcome():
    if not "sentence" in welcome and request.method == 'GET':
        return "", 202
    if request.method == 'POST' and "new_welcome" in request.get_json():
        welcome["sentence"] = request.get_json()["new_welcome"]
        return "", 202
    return welcome
    
@app.route('/api/welcome/<int:index>', methods=['GET', 'PATCH'])
def api_welcome_index(index):
    if index > len(welcome) :
        abort(404)
    if request.method == 'PATCH' and "new_welcome" in request.get_json():
        welcome["sentence"][index] = "prout"

    return jsonify({"index": index, "val": welcome["sentence"][index]})

@app.route('/api/request/', methods=['GET', 'POST'])
@app.route('/api/request/<path>', methods=['GET','POST'])
def api_request(path=None):
    resp = {
            "method":   request.method,
            "url" :  request.url,
            "path" : path,
            "args": request.args,
            "headers": dict(request.headers),
    }
    if request.method == 'POST':
        resp["POST"] = {
                "data" : request.get_json(),
                }
        return "", 202
    return jsonify(resp)

@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404
