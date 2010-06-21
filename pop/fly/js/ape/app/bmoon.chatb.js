//; if (!bmoon) bmoon = {};
; var bmoon = bmoon || {};
bmoon.chat = {
	version: '1.0',
	ape: {},
	groupPipe: null,
	cUserID: '0',
	usersOn: ["0"],

	init: function(ape) {
		var o = bmoon.chat;

		if (o.inited) return o;
		o.inited = true;

		o.m = $('#chat-msg-text');
		o.btm = $('#chat-msg-submit');
		o.userlist = $('#im-user-list > ul');
		o.msglist = $('#im-msgs');
		
		o.ape = ape;

		return o;
	},

	onready: function(ape, pipe) {
		var o = bmoon.chat.init(ape);

		o.groupPipe = pipe;
		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.chat.init();

		o.m.bind('keydown', 'ctrl+return', o.msgSend);
		o.btm.click(o.msgSend);
		$('.im-user-list li').click(o.openChat);
	},

	msgSend: function() {
		var o = bmoon.chat.init();
		
		var
		mv = o.m.val(),
		pipe = o.ape.lcsCurrentPipe;

		if (!mv.length) return;
		o.m.val('');

		if (pipe && ($.inArray(o.cUserID, o.usersOn) != -1)) {
			pipe.request.send('LCS_SEND', {msg: m});
		} else {
			o.ape.request.send('LCS_MSG', {uname: o.ape.lcsaname, msg: m});
		}
	},

	openChat: function() {
		var o = bmoon.chat.init();
		
		var
		c = $(this),
		id = c.attr('uname'),
		pubid = c.attr('pubid'),
		msgbox = $('#im-msg-'+id);

		$('#im-user-'+o.cUserID).removeClass('cur').click(o.openChat);
		$('#im-msg-'+o.cUserID).hide();
		o.cUserID = id;
		
		c.addClass('cur').unbind('click');
		if (!msgbox.length) {
			var html = '<div id="im-msg-'+id+'"></div>';
			msgbox = $(html).appendTo(o.msglist);
			o.reqHistory({uname: id});
		}
		msgbox.show();

		if (pubid.length) {
			o.ape.lcsCurrentPipe = ape.getPipe(pubid);
		} else if (o.cUserID == "0") {
			o.ape.lcsCurrentPipe = o.groupPipe;
		} else {
			o.ape.lcsCurrentPipe = null;
		}
	},

	// {uname: "defTxg", pubid: "esfes323sdfssdf32r"}
	userOn: function(data) {
		var o = bmoon.chat.init();

		var c = $('#im-user-' + data.uname);
		if (!c.length) {
			c = $('<li id="im-user-'+data.uname+'" uname="'+
					data.uname+'">'+data.uname+'</li>');
			c.appendTo(o.userlist);
		}
		c.removeClass('off').addClass('on');
		c.attr('pubid', data.pubid);
		o.usersOn.push(data.uname);
	},

	userOff: function(data) {
		var o = bmoon.chat.init();

		var c = $('#im-user-' + data.uname);
		if (c.length) {
			c.removeClass('on').addClass('off').attr('pubid', '');
		}
		o.usersOn.splice($.inArray(data.uname, o.usersOn), 1);
	},

	// {uname: "defTxg"}
	reqHistory: function(data) {
		var o = bmoon.chat.init();
		
	}
};
