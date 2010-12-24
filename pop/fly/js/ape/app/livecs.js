// new liveCS(ape).initialize({aname: document.domain});
function liveCS(ape) {
	var ui = bmoon.chat;
	this.initialize = function(opts) {
		/*
		 * 我们本可设置 this.xxx = yyy, 这样， 只要是本实例成员函数发起的调用，都可以访问。
		 * 但， LiveCS实例的函数有可能被其它 obj 发起调用(如addEvent())，
		 * 这时 this.xxx 变得不可访问，
		 * 故， 我们只能把 xxx 设置到 ape 里面， 无论谁发起的成员函数调用， 都可访问到 ape.xxx
		 */

		// app name
		ape.lcspname = opts.aname || document.domain;
		ape.lcsaname = null;
		ape.defaultUI = opts.defaultUI || 'max';
		ape.restoreUI = opts.restoreUI || true;

		// user name
		ape.lcsuname = Cookie.read("lcs_uname");
		if (!ape.lcsuname) {
			ape.lcsuname = bmoon.utl.randomName();
			Cookie.write("lcs_uname", ape.lcsuname,
						 {'path': '/', 'duration': 36500});
		}

		// user visit this app's time
		ape.lcsutime = Cookie.read("lcs_utime");
		if (!ape.lcsutime) {
			ape.lcsutime = 0;
		}
		ape.lcsutime = parseInt(ape.lcsutime)+1;
		if (!ape.options.restore) {
			Cookie.write("lcs_utime",
						 ape.lcsutime, {'path': '/', 'duration': 36500});
		}

		ape.lcsCurrentPipe = null;

		ape.onRaw("RAW_RECENTLY", this.rawLcsRecently);
		ape.onRaw("LCS_DATA", this.rawLcsData);

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
				'aname': ape.lcspname, 'utime': ape.lcsutime,
				'url': location.href, 'title': document.title}, opt);
		}
		ape.request.stack.send();
	};

	this.pipeCreate = function(pipe, options) {
		if (pipe.properties.pname == ape.lcspname) {
			ape.lcsaname = pipe.properties.aname;
			ui.init(ape);
			// send LCS_VISIT only on session restore
			if (ape.options.restore) {
				ape.request.send("LCS_VISIT", {
					'aname': ape.lcsaname, 'pname': ape.lcspname,
					'url': location.href, 'title': document.title
				});
			}
			$.getJSON('http://www.kaiwuonline.com/json/msg?JsonCallback=?',
					  {name: ape.lcsaname, name2: ape.lcsuname},
					  function(data) {
						  if (data.success == 1 && bmoon.utl.type(data.raws) == 'Array') {
							  $.each(data.raws, function(i, v) {
								  var raw = JSON.parse(v);
								  if (raw) {
									  ape.callRaw(raw);
								  }
							  });
						  }
					  });
		}
	};

	this.createUser = function(user, pipe) {
		if (pipe.properties.pname == ape.lcspname && user.properties.isadmin) {
			ape.setSession({'lcsCurrentPipe': user.pubid});
			//ape.lcsCurrentPipe = user.pipes;
			ape.lcsCurrentPipe = ape.getPipe(user.pubid);
			ui.adminOn({pname: ape.lcspname, aname: pipe.properties.aname});
		}
	};

	this.deleteUser = function(user, pipe) {
		if (pipe.properties.pname == ape.lcspname && user.properties.isadmin) {
			ape.lcsCurrentPipe = null;
			ui.adminOff({pname: ape.lcspname, aname: pipe.properties.aname});
		}
	};

	this.rawLcsRecently = function(raw, pipe) {
		if ((raw.data.from && raw.data.from.properties.uin == ape.lcsuname) ||
			raw.data.to_uin == ape.lcsuname ||
			(raw.data.to && raw.data.to.properties.uin == ape.lcsuname)) {
			var
			type = raw.data.type,
			
			dt = new Date(raw.time*1000),
			y = dt.getFullYear(), m = dt.getMonth()+1, d = dt.getDate(),
			h = dt.getHours(), mt = dt.getMinutes(), s = dt.getSeconds(),
			tm = y+"-"+m+"-"+d+" "+h+":"+mt+":"+s,
			
			from = raw.data.from.properties.uin,
			to = raw.data.to_uin ||	(raw.data.to && raw.data.to.properties.uin);

			// filter join and visit messages
			if (type == 'send' || type == 'msg') {
				ui.onRecently({from: from, to: to, type: type, tm: tm, data: raw.data});
			}
		}
	};

	this.rawLcsData = function(raw, pipe) {
		if ((raw.data.from && raw.data.from.properties.uin == ape.lcsaname) ||
			pipe == ape.lcsCurrentPipe) {
			var
			type = raw.data.type,
			stm = new Date(raw.time*1000).toString(),
			tm = stm.match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0],
			from = raw.data.from.properties.uin;
			
			ui.onData({from: from, type: type, tm: tm, data: raw.data});
		}
	};
}
