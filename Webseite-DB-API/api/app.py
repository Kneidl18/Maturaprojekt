from flask import Flask, jsonify, request,send_file
from flask_cors import CORS

app = Flask(__name__)
CORS(app)


@app.route("/test/get", methods=['GET'])
def api_test_get():
    return jsonify({
        'message': "This test was successful"
        })


@app.route("/test/post", methods=['POST'])
def api_test_post():
    print(request.is_json)
    content = request.get_json()
    print(content)
    print(content['test'])
    return 'JSON posted'


@app.route("/test/put", methods=['PUT'])
def api_test_put():
    print(request.is_json)
    content = request.get_json()
    print(content)
    return 'JSON put'


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=8000)
