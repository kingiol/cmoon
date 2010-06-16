function liveCS(ape) {
	var ui = bmoon.chat;
	this.initialize = function(opts) {
		// app name
		ape.lcsaname = Cookie.read('aname');
		ape.lcsmasn = Cookie.read('masn');

		if (!ape.lcsaname || !ape.lcsmasn) return;

		ape.lcsCurrentPipe = null;

		ape.onRaw("ident", this.rawLcsIdent);
		ape.onRaw("lcsdata", this.rawLcsData);

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
		if (ape.options.restore) {
			ape.getSession('lcsCurrentPipe', function(resp) {
				ape.lcsCurrentPipe = ape.getPipe(resp.data.sessions.lcsCurrentPipe);
			}, opt);
		} else {
			ape.request.stack.add("LCS_JOINB", {
				'aname': ape.lcsaname, 'masn': ape.lcsmasn}, opt);
		}
		ape.request.stack.send();
	};

	this.pipeCreate = function(pipe, options) {
		if (pipe.properties.name.toLowerCase() == "livecspipe_" + ape.lcsaname) {
			if (!ape.lcsCurrentPipe) {
				ape.lcsCurrentPipe = pipe;
				ape.setSession({'lcsCurrentPipe': pipe.getPubid()});
			}
			ui.onready(ape);
		}
	};

	this.createUser = function(user, pipe) {
	};

	this.deleteUser = function(user, pipe) {
	};

	this.rawLcsIdent = function(raw, pipe) {
	};

	this.rawLcsData = function(raw, pipe) {
		if (pipe == ape.lcsCurrentPipe) {
			ui.onmsg(raw);
			var msg = unescape(raw.data.msg);
			var uin = raw.data.from.properties.uin;
			var tm = Date(eval(raw.time)).match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0];
			//bmoon.lcs.userSaid(ape, uin, msg, tm);
		}
	};
}

$(document).ready(function() {
	var client = new APE.Client();
	client.load({
		identifier: 'lcs',
		transport: 2,
		complete: function(ape) {
			new liveCS(ape).initialize({aname: document.domain});
		}
	});
});
