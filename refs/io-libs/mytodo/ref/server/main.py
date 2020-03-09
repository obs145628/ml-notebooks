import copy
import os

import flask
from flask import request, jsonify



class Db:

    def __init__(self):
        self.data = [
            {'id': 0, 'title': 'clean house', 'details': 'do it now', 'level': 5},
            {'id': 1, 'title': 'make fire', 'details': 'use everburning wood', 'level': 7},
            {'id': 2, 'title': 'take a nap', 'details': 'on my new bed', 'level': 4},
        ]

    def get_all(self):
        return [{'id': x['id'], 'title': x['title']} for x in self.data]

    def get_task(self, id):
        for t in self.data:
            if t['id'] == id:
                return t
        return None

    def add_task(self, title, details, level):
        task = {
            'id': 0 if len(self.data) == 0 else self.data[-1]['id'] + 1,
            'title': title,
            'details': details,
            'level': level,
        }
        self.data.append(task)
        return task

    def update_task(self, task):
        t = self.get_task(task['id'])
        t['title'] = task['title']
        t['details'] = task['details']
        t['level'] = task['level']

    def remove_task(self, task):
        t = self.get_task(task['id'])
        self.data.remove(t)


PATH_WWW = os.path.join(os.path.dirname(__file__), '../www')

app = flask.Flask(__name__,
                  static_url_path='', 
                  static_folder=PATH_WWW
)

app.config["DEBUG"] = True


#@app.route('/', methods=['GET'])
#def home():
#    return "<h1>Distant Reading Archive</h1><p>This site is a prototype API for distant reading of science fiction novels.</p>"

db = Db()


def api_err(mess, code=400):
    return flask.make_response(flask.jsonify({'error': 'Invalid API request: `{}`'.format(mess)}), code)

@app.route('/api/v1/tasks/', methods=['GET'])
def get_all():
    res = {'tasks': db.get_all()}
    return jsonify(res)


@app.route('/api/v1/tasks/<int:task_id>', methods=['GET'])
def get_task(task_id):
    task = db.get_task(task_id)
    if task is None:
        return api_err('cannot get task: not found')
    else:
        return jsonify({'task': task})

@app.route('/api/v1/tasks/', methods=['POST'])
def add_task():
    if not request.json or not 'title' in request.json or not 'details' in request.json or not 'level' in request.json:
        return api_err('invalid post request for tasks')

    task = db.add_task(request.json['title'], request.json['details'], request.json['level'])
    return jsonify({'task': task}), 201

@app.route('/api/v1/tasks/<int:task_id>', methods=['PUT'])
def update_task(task_id):
    if not request.json or not 'title' in request.json or not 'details' in request.json or not 'level' in request.json:
        return api_err('invalid put request for tasks')
    task = db.get_task(task_id)
    if task is None:
        return api_err('cannot update task: not found')

    task['title'] = request.json['title']
    task['details'] = request.json['details']
    task['level'] = request.json['level']
    db.update_task(task)
    return jsonify({'task': task})

@app.route('/api/v1/tasks/<int:task_id>', methods=['DELETE'])
def delete_task(task_id):
    task = db.get_task(task_id)
    if task is None:
        return api_err('cannot get task: not found')
    else:
        db.remove_task(task)
        return jsonify({'task': task})

app.run()
