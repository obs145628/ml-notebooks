
API_URL = '/api/v1/tasks/';

function json_req(url, method, cb, data) {

    var req = {
	url: url,
	method: method,
	dataType: "json",
    };

    if (data) {
	req.data = JSON.stringify(data);
	req.contentType = 'application/json; charset=utf-8';
    }
    
    $.ajax(req)
	.done(function(data) { cb(null, data); })
	.fail(function(xhr, data) {cb({xhr: xhr, data: data}, null);});
}

var Db = {
    get_all: function(cb) {
	json_req(API_URL, 'GET', cb);
    },

    get_task: function(id, cb) {
	json_req(API_URL + id, 'GET', cb);
    },

    get_all_full: function(cb) {
	var res = [];
	var total = 0;
	
	Db.get_all(function(err, data) {
	    if (err)
		return cb(err, null);

	    total = data.tasks.length;
	    data.tasks.forEach(function(t) {
		Db.get_task(t.id, function(err, data) {
		    if (err)
			return cb(err, null);

		    res.push(data.task);
		    if (res.length == total)
			cb(null, res);
	    });
	});
	
    });
    },

    add_task: function(title, details, level, cb) {
	json_req(API_URL, 'POST', cb, {
	    title: title,
	    details: details,
	    level: level,
	});
    },

    update_task: function(id, new_title, new_details, new_level, cb) {
	json_req(API_URL + id, 'PUT', cb, {
	    title: new_title,
	    details: new_details,
	    level: new_level,
	});
    },

    delete_task: function(id, cb) {
	json_req(API_URL + id, 'DELETE', cb);
    },
    
};



$(function() {
    var T_TASKS_TABLE = document.getElementById("tasks-table");
    var T_TASKS_TABLE_BODY = document.getElementById("tasks-table-body");
    var T_TASKS_EMPTY = document.getElementById("tasks-empty");
    var T_IN_TITLE_NEW = document.getElementById("in-title-new");
    var T_IN_DETAILS_NEW = document.getElementById("in-details-new");
    var T_IN_LEVEL_NEW = document.getElementById("in-level-new");
    var T_BTN_ADD = document.getElementById("btn-add");
    

    function gui_add_task(task) {
	var el = document.createElement('tr');
	el.id = 'task-row-' + task.id;
	el.innerHTML = ''
	    + '<th scope="row">' + task.id + '</th>'
	    + '<td><input type="text" value="' + task.title + '" id="in-title-' + task.id + '" /></td>'
	    + '<td><input type="text" value="' + task.details + '" id="in-details-' + task.id + '" /></td>'
	    + '<td><input type="text" value="' + task.level + '" id="in-level-' + task.id + '" /></td>'
	    + '<td><input type="button" value="update" id="btn-set-' + task.id + '" /><input type="button" value="delete" id="btn-del-' + task.id + '" /></td>';
	T_TASKS_TABLE_BODY.appendChild(el);

	var in_title = document.getElementById("in-title-" + task.id);
	var in_details = document.getElementById("in-details-" + task.id);
	var in_level = document.getElementById("in-level-" + task.id);
	var btn_set = document.getElementById("btn-set-" + task.id);
	var btn_del = document.getElementById("btn-del-" + task.id);

	btn_set.addEventListener('click', function() {
	    var id = task.id;
	    var new_title = in_title.value;
	    var new_details = in_details.value;
	    var new_level = parseInt(in_level.value);
	    Db.update_task(id, new_title, new_details, new_level, function(err) {
		if (err)
		    console.error(err);
	    });
	}, false);

	btn_del.addEventListener('click', function() {
	    Db.delete_task(task.id, function(err) {
		if (err)
		    return console.error(err);

		gui_del_task(task);
	    });
	}, false);
    }

    function gui_del_task(task) {
	var el = document.getElementById('task-row-' + task.id);
	T_TASKS_TABLE_BODY.removeChild(el);
    }
    

    T_TASKS_TABLE.style.display = "none";

    Db.get_all_full(function(err, data) {
	if (err)
	    return console.error(err);
	if (data.length == 0)
	    return;
	
	T_TASKS_TABLE.style.display = "block";
	T_TASKS_EMPTY.style.display = "none";
	data.forEach(gui_add_task);
    });

    T_BTN_ADD.addEventListener('click', function() {
	var new_title = T_IN_TITLE_NEW.value;
	var new_details = T_IN_DETAILS_NEW.value;
	var new_level = parseInt(T_IN_LEVEL_NEW.value);

	Db.add_task(new_title, new_details, new_level, function(err, data) {
	    if (err)
		return console.error(err);

	    gui_add_task(data.task);
	    T_IN_TITLE_NEW.value = "";
	    T_IN_DETAILS_NEW.value = "";
	    T_IN_LEVEL_NEW.value = "";
	});

	
    }, false);
});
