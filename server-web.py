import logging, argparse, subprocess, signal, time, os, sys
from flask import Flask, json, jsonify, abort, request, make_response
from flask_restful import Api, Resource, reqparse
from flask_httpauth import HTTPBasicAuth
from flask_cors import CORS
from func import get_address, make_index_html, make_success_html, make_failure_html, make_valid_html

app = Flask(__name__)
app.config['CORS_HEADERS'] = 'Content-Type'
api = Api(app)
auth = HTTPBasicAuth()

cors = CORS(app)

def signal_handler(sig, frame):
    if os.path.exists("address"):
        os.remove("address")
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

# URI: /
# HTTP behavior: GET
# GET: Get the available chain identifiers (e.g., year2019)
class Webserver(Resource):
    def get(self):
        logging.info("index: {}".format(index))
        return make_response(index)

    def post(self):
        password = request.form.get("password")
        logging.info("password: {}".format(password))
        cmd = ["./client", "--addr", "127.0.0.1", "--port", "5555", "--account", "tmp", "--password", password]
        logging.info("cmd: {}".format(cmd))
        proc = subprocess.Popen(cmd, stdout=subprocess.PIPE)
        output = proc.stdout.read().decode()
        if password == "correctpassword":
            logging.info("Login success")
            return make_response(valid)
        elif "the root privilege" in output:
            logging.info("Login success")
            return make_response(success)
        else:
            logging.info("Login failure")
            return make_response(failure)

# The assignment of the mapping between the URI and the related class
api.add_resource(Webserver, '/')

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", metavar="<web server's listening port>", help="Web server's listening port", type=int, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)
    aname = "address"

    if not os.path.exists(aname):
        cmd = ["./client", "--addr", "127.0.0.1", "--port", "5555", "--account", "tmp", "--password", "correctpassword"]
        proc = subprocess.Popen(cmd)
        time.sleep(1)

    global index
    global success
    global failure
    global valid

    addr = get_address(aname)
    index = make_index_html(addr)
    success = make_success_html(addr)
    failure = make_failure_html(addr)
    valid = make_valid_html(addr)

    app.run(host="0.0.0.0", port=args.port)

if __name__ == "__main__":
    main()
