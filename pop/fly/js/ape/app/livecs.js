// new liveCS(ape).initialize({aname: document.domain});
function liveCS(ape) {
	var ui = bmoon.lcs.chat;
	this.initialize = function(opts) {
		/*
		 * 我们本可设置 this.xxx = yyy, 这样， 只要是本实例成员函数发起的调用，都可以访问。
		 * 但， LiveCS实例的函数有可能被其它 obj 发起调用(如addEvent())，这时 this.xxx 变得不可访问，
		 * 故， 我们只能把 xxx 设置到 ape 里面， 无论谁发起的成员函数调用， 都可访问到 ape.xxx
		 */

		// app name
		ape.lcsaname = opts.aname || document.domain;

		// user name
		ape.lcsuname = Cookie.read("lcs_uname");
		if (ape.lcsuname == null) {
			ape.lcsuname = bmoon.utl.randomWord(8);
			Cookie.write("lcs_uname", ape.lcsuname, {'path': '/', 'duration': 36500});
		}

		// user visit this app's time
		ape.lcsutime = Cookie.read("lcs_utime");
		if (!ape.lcsutime) {
			ape.lcsutime = 0;
		}
		ape.lcsutime = parseInt(ape.lcsutime)+1;
		if (!ape.options.restore) {
				Cookie.write("lcs_utime", ape.lcsutime, {'path': '/', 'duration': 36500});
		}

		ape.lcsCurrentPipe = null;

		ape.onRaw("ident", this.rawLcsIdent);
		ape.onRaw("lcsdata", this.rawLcsData);

		ape.onError("110", ape.clearSession);
		ape.onError("111", ape.clearSession);
		ape.onError("112", ape.clearSession);
		ape.onError("113", ape.clearSession);

		ape.addEvent("userJoin", this.createUser);
		ape.addEvent("userLeft", this.deleteUser);
		ape.addEvent("multiPipeCreate", this.pipeCreate);

		ape.addEvent("load", this.start);
	};

	this.start = function() {
		var opt = {'sendStack': false, 'request': 'stack'};
		ape.start({'uin': ape.lcsuname}, opt);
		if (ape.options.restore) {
			ape.getSession('lcsCurrentPipe', function(resp) {
							   ape.lcsCurrentPipe = ape.getPipe(resp.data.sessions.lcsCurrentPipe);
						   }, opt);
		} else {
			ape.request.stack.add("LCS_JOIN", {
									  'aname': ape.lcsaname, 'utime': ape.lcsutime,
									  'url': location.href, 'title': document.title}, opt);
		}
		ape.request.stack.send();
	};

	this.pipeCreate = function(pipe, options) {
		if (pipe.properties.name.toLowerCase() == "livecspipe_" + ape.lcsaname) {
			ui.init(ape);
		}
	};

	this.createUser = function(user, pipe) {
		if (user.properties.isadmin) {
			ape.lcsCurrentPipe = pipe;
			ape.setSession({'lcsCurrentPipe': pipe.getPubid()});
			ui.adminOn(user);
		}
	};

	this.deleteUser = function(user, pipe) {
		if (user.properties.isadmin) {
			ape.lcsCurrentPipe = null;
			ui.adminOff(user);
		}
	};

	this.rawLcsIdent = function(raw, pipe) {
		var jid = parseInt(raw.data.user.properties.jid);
		// send LCS_VISIT only on session restore
		if (jid && ape.options.restore) {
			ape.request.send("LCS_VISIT", {'jid': jid, 'url': location.href, 'title': document.title});
		}
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
