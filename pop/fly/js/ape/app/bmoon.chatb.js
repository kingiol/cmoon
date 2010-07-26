//; if (!bmoon) bmoon = {};
; var bmoon = bmoon || {};
bmoon.chat = {
	version: '1.0',
	ape: {},
	groupPubid: null,
	cUserID: '0',
	usersOn: ["0"],
	usersFetched: ["0"],

	init: function(ape) {
		var o = bmoon.chat;

		if (o.inited) return o;
		o.inited = true;

		var rmdhtml = $.browser.msie? '<bgsound id="remind-sound"></bgsound>': '<audio id="remind-sound"></audio>';
		
		$(rmdhtml).appendTo('body');
		o.m = $('#chat-msg-text');
		o.btm = $('#chat-msg-submit');
		o.userlist = $('#im-users > ul');
		o.msglist = $('#im-msgs');
		o.stat = $('#im-stat');
		o.reminder = $('#remind-sound')[0];
		o.rmdsw = $('#remind-sound-sw');
		
		o.ape = ape;

		return o;
	},

	_nodeUser: function(uname, create) {
		var o = bmoon.chat.init();

		var
		r = $('#im-user-' + uname),
		html = '<li id="im-user-'+uname+'" uname="'+uname+'">'+uname+'</li>';
		
		if (!r.length && create) {
			r = $(html).appendTo(o.userlist).click(o.openChat);
		}
		return r;
	},

	_nodeMsg: function(uname, create) {
		var o = bmoon.chat.init();
		
		var
		r = $('#im-msg-' + uname),
		html = [
			'<div id="im-msg-', uname, '">',
			    '<div class="recently"></div>',
			    '<div class="data"></div>',
			'</div>'
		].join('');
		
		if (!r.length && create) {
			r = $(html).appendTo(o.msglist);
			if (o.cUserID != uname) r.hide();
		}
		return r;
	},

	_strAction: function(type, data) {
		if (data.ref) data.sref = '通过 ' + decodeURIComponent(data.ref);
		else data.sref = '';
		
		var
		msg = decodeURIComponent(data.msg).replace(/<\/?[^>]*>/g, ''),
		r = {
			'join':
			data.sref + ' 来到本站, 访问了页面<a target="_blank" href="'+
				decodeURIComponent(data.url)+'">'+
				decodeURIComponent(data.title)+'</a>',
			
			'visit':
			'打开了页面 <a target="_blank" href="'+
				decodeURIComponent(data.url)+'">'+
				decodeURIComponent(data.title)+'</a>',

			'left': '离开了本站',
			'send': msg,
			'msg': '留言说： ' + msg
		};
		return r[type];
	},

	// {from: from, type: type, tm: tm, data: data}
	_strMsg: function(data) {
		var o = bmoon.chat.init();

		var uname = data.from == o.ape.lcsaname ? '我': data.from;
		
		return [
			'<div class="item-', data.type, '">',
			    '<span class="item-name">',	decodeURIComponent(uname),'</span>',
			    '<span class="item-time">',	data.tm,'</span>',
			    '<span class="item-content">', o._strAction(data.type, data.data),
			    '</span>',
			'</div>'
		].join('');
	},

	onready: function(ape, pubid) {
		var o = bmoon.chat.init(ape);

		o.groupPubid = pubid;
		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.chat.init();

		o.m.bind('keydown', 'ctrl+return', o.msgSend);
		o.btm.click(o.msgSend);
		$('.im-users li').click(o.openChat);
	},

	msgSend: function() {
		var o = bmoon.chat.init();
		
		var
		mv = o.m.val(),
		pipe = o.ape.lcsCurrentPipe,
		type = $.inArray(o.cUserID, o.usersOn) != -1 ? 'send': 'msg',
		databox = $('.data', o._nodeMsg(o.cUserID, true)),
		html = o._strMsg({
			from: o.ape.lcsaname,
			type: type,
			tm: Date().match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0],
			data: {msg: mv}
		});

		if (!mv.length) return;
		o.m.val('');

		$(html).appendTo(databox);
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;

		if (pipe && type == 'send') {
			pipe.request.send('LCS_SEND', {msg: mv});
		} else {
			o.ape.request.send('LCS_MSG', {uname: o.ape.lcsaname, msg: mv});
		}

		o.soundRemind('send');
	},

	openChat: function() {
		var o = bmoon.chat.init();
		
		var
		c = $(this),
		id = c.attr('uname'),
		pubid = c.attr('pubid'),
		userbox = o._nodeUser(o.cUserID, false),
		msgbox = o._nodeMsg(o.cUserID, false);

		userbox.removeClass('cur').click(o.openChat);
		msgbox.hide();
		o.cUserID = id;
		
		c.removeClass('dirty').addClass('cur').unbind('click');
		o._nodeMsg(id, true).fadeIn();
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		if (id == '0') {
			o.stat.html("给所有在线访客发送消息");
		} else {
			o.stat.html("与 " + id + " 对话");
		}

		if ($.inArray(id, o.usersFetched) == -1) {
			o.ape.request.send('LCS_RECENTLY', {uin: id, type: 1});
			o.usersFetched.push(id);
		}

		if (pubid && pubid.length) {
			o.ape.lcsCurrentPipe = o.ape.getPipe(pubid);
		} else if (o.cUserID == '0') {
			o.ape.lcsCurrentPipe = o.ape.getPipe(o.groupPubid);
		} else {
			o.ape.lcsCurrentPipe = null;
		}
	},

	// {defTxg: 1, eoogG: 1}
	dearUsers: function(data) {
		var o = bmoon.chat.init();

		if (bmoon.utl.type(data) == 'Array') {
			$.each(data, function(i, val){
				if (bmoon.utl.type(val) == 'Object') {
					$.each(val, function(k, v) {
						var c = o._nodeUser(k, true);
						if (k != o.cUserID) c.addClass('dirty');
					});
				} else if (bmoon.utl.type(val) == 'String') {
					var c = o._nodeUser(val, true);
					if (val != o.cUserID) c.addClass('dirty');
				}
			});
		}
	},
	
	// {uname: 'defTxg', pubid: 'esfes323sdfssdf32r'}
	userOn: function(data) {
		var o = bmoon.chat.init();

		var c = o._nodeUser(data.uname, true);
		c.removeClass('off').addClass('on');
		c.attr('pubid', data.pubid);
		o.usersOn.push(data.uname);
	},

	// {uname: user.properties.uin}
	userOff: function(data) {
		var o = bmoon.chat.init();

		var
		userbox = o._nodeUser(data.uname, false),
		databox = $('.data', o._nodeMsg(data.uname, false)),
		html = o._strMsg({
			from: data.uname,
			type: 'left',
			tm: Date().match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0],
			data: {}
		});
		
		if (userbox.length) {
			userbox.removeClass('on').addClass('off').attr('pubid', '');
		}
		o.usersOn.splice($.inArray(data.uname, o.usersOn), 1);

		// for history raw, use event_onleft
		/*
		$(html).appendTo(databox);
		if (o.cUserID == data.uname) {
			o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		}
		*/
	},

	// {from: from, type: type, tm: tm, data: data}
	onData: function(data) {
		var o = bmoon.chat.init();

		var
		uname = data.from,
		userbox = o._nodeUser(uname, true),
		databox = $('.data', o._nodeMsg(uname, true)),
		html = o._strMsg(data);

		if (data.type == 'send') {
			o.soundRemind('receive');
		} else if (data.type == 'join') {
			o.soundRemind('login');
		} else if (data.type == 'left') {
			o.soundRemind('logout');
		}
		
		if (o.cUserID != uname) {
			userbox.addClass('dirty');
			// avoid double messages appear
			if ($.inArray(uname, o.usersFetched) == -1) return;
		}
		
		$(html).appendTo(databox);
		if (o.cUserID == uname) {
			o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		}
	},

	// {from: from, to: to, type: type, tm: tm, data: data}
	onRecently: function(data) {
		var o = bmoon.chat.init();

		var
		uname = data.from == o.ape.lcsaname ? data.to: data.from,
		userbox = o._nodeUser(uname, true),
		recentbox = $('.recently', o._nodeMsg(uname, true)),
		html = o._strMsg(data);
		
		$(html).appendTo(recentbox);
		if (o.cUserID == uname) {
			o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		}
	},

	soundRemind: function(type) {
		var o = bmoon.chat.init();

		if (o.rmdsw.attr('checked') == true) {
			o.reminder.src = '/obj/audio/'+type+'.wav';
			if (!$.browser.msie) {
				o.reminder.load();
				o.reminder.play();
			}
		}
	}
};
