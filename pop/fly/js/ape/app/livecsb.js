function liveCS(ape) {
	var ui = bmoon.chat;
	this.initialize = function() {
		// app name
		ape.lcsaname = Cookie.read('aname');
		ape.lcsmasn = Cookie.read('masn');
		ape.lcsCurrentPipe = null;
		
		if (!ape.lcsaname || !ape.lcsmasn) return;

		ape.onRaw("ident", this.rawLcsIdent);
		ape.onRaw("LCS_DEARUSERS", this.rawLcsDearusers);
		ape.onRaw("RAW_RECENTLY", this.rawLcsRecently);
		ape.onRaw("LCS_DATA", this.rawLcsData);

		ape.onError("110", ape.clearSession);
		ape.onError("111", ape.clearSession);
		ape.onError("112", ape.clearSession);
		ape.onError("113", ape.clearSession);

		ape.onError("210", ape.clearSession);
		ape.onError("211", ape.clearSession);
		
		ape.addEvent("userJoin", this.createUser);
		ape.addEvent("userLeft", this.deleteUser);
		ape.addEvent("multiPipeCreate", this.pipeCreate);

		ape.addEvent("load", this.start);
	};

	this.start = function() {
		var opt = {'sendStack': false, 'request': 'stack'};
		ape.start({'uin': ape.lcsaname}, opt);
		if (!ape.options.restore) {
			ape.request.stack.add("LCS_JOINB", {
				'aname': ape.lcsaname, 'masn': ape.lcsmasn}, opt);
		}
		ape.request.stack.send();
	};

	this.pipeCreate = function(pipe, options) {
		if (pipe.properties.aname == ape.lcsaname) {
			ape.lcsCurrentPipe = pipe;
			ui.onready(ape, pipe);
			ape.request.stack.add('LCS_DEARUSERS', null);
			ape.request.stack.add('LCS_RECENTLY', {uin: "0", type: 0});
			ape.request.stack.send();
			//ape.request.send();
		}
	};

	this.createUser = function(user, pipe) {
		if (pipe.properties.aname == ape.lcsaname &&
			user.properties.uin != ape.lcsaname) {
			ui.userOn({uname: user.properties.uin, pubid: user.pubid});
		}
	};

	this.deleteUser = function(user, pipe) {
		if (pipe.properties.aname == ape.lcsaname &&
			user.properties.uin != ape.lcsaname) {
			ui.userOff({uname: user.properties.uin});
		}
	};

	this.rawLcsIdent = function(raw, pipe) {
	};

	this.rawLcsDearusers = function(raw, pipe) {
		//var users = [];
		//$.each(raw.data, function(key, val){
		//	users.push(key);
		//});
		//ui.dearUsers(users);
		ui.dearUsers(raw.data);
	};

	this.rawLcsRecently = function(raw, pipe) {
		// some offline & notice message don't have data.to.properties.uin,
		// use data.to_uin instead
		if (raw.data.to_uin == ape.lcsaname ||
			raw.data.to.properties.uin == ape.lcsaname) {
			var type = raw.data.type;
			var tm = Date(eval(raw.time*1000)).match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0];
			var uname = raw.data.from.properties.uin;
			ui.onRecently({uname: uname, type: type, tm: tm, data: raw.data});
		}
	};

	this.rawLcsData = function(raw, pipe) {
		// some notice message don't have data.to.properties.uin,
		// use data.to_uin instead
		if (raw.data.to_uin == ape.lcsaname ||
			raw.data.to.properties.uin == ape.lcsaname) {
			var type = raw.data.type;
			var tm = Date(eval(raw.time*1000)).match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0];
			var uname = raw.data.from.properties.uin;
			ui.onData({uname: uname, type: type, tm: tm, data: raw.data});
		}
	};
}

$(document).ready(function() {
	var client = new APE.Client();
	client.load({
		identifier: 'lcs',
		transport: 2,
		complete: function(ape) {
			new liveCS(ape).initialize();
		}
	});
});
