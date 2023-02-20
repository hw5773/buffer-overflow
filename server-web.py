import os, sys, logging
import argparse
import subprocess
from flask import Flask, json, jsonify, abort, request, make_response
from flask_restful import Api, Resource, reqparse
from flask_httpauth import HTTPBasicAuth
from flask_cors import CORS

app = Flask(__name__)
app.config['CORS_HEADERS'] = 'Content-Type'
api = Api(app)
auth = HTTPBasicAuth()

cors = CORS(app)

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
    parser.add_argument("-i", "--index", metavar="<index file path>", help="Index file path", type=str, required=True)
    parser.add_argument("-s", "--success", metavar="<success file path>", help="Success file path", type=str, required=True)
    parser.add_argument("-f", "--failure", metavar="<failure file path>", help="Failure file path", type=str, required=True)
    parser.add_argument("-v", "--valid", metavar="<valid file path>", help="Valid file path", type=str, required=True)
    parser.add_argument("-p", "--port", metavar="<web server's listening port>", help="Web server's listening port", type=int, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    logging.basicConfig(level=args.log)

    if os.path.exists(args.index) and os.path.exists(args.success) and os.path.exists(args.failure) and os.path.exists(args.valid):
        global index
        global success
        global failure
        global valid

        with open(args.index, "r") as f:
            index = f.read()

        with open(args.success, "r") as f:
            success = f.read()

        with open(args.failure, "r") as f:
            failure = f.read()

        with open(args.valid, "r") as f:
            valid = f.read()

        app.run(host="0.0.0.0", port=args.port)
    else:
        logging.error("Some of the html files do not exist. Please insert the correct ones.")
        logging.error("  - Index file: {} ({})".format(args.index, os.path.exists(args.index)))
        logging.error("  - Success file: {} ({})".format(args.success, os.path.exists(args.success)))
        logging.error("  - Failure file: {} ({})".format(args.failure, os.path.exists(args.failure)))
        logging.error("  - Valid file: {} ({})".format(args.valid, os.path.exists(args.valid)))

if __name__ == "__main__":
    main()
