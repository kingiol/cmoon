//; if (!bmoon) bmoon = {};
; var bmoon = bmoon || {};
bmoon.chat = {
	version: "1.0",
	adminuser: {},
	ape: {},

    debug: function(msg) {
        $('<div>'+ msg +'</div>').appendTo('body');
    },
	
	init: function(ape) {
		var o = bmoon.chat;
		if (o.inited) return o;

		var
		html = [
			'<div id="bchat">',
				'<div id="bchat-head"><a href="javascript: void(0);" id="bchat-trigger">留言</a></div>',
				'<div id="bchat-body">',
					'<div id="bchat-msgs"></div>',
					'<textarea cols="27" rows="3" id="bchat-input"></textarea>',
					'<div><a title="Control+Enter" href="javascript:" id="bchat-snd">发送</a></div>',
				'</div>',
			'</div>'
		].join('');

		o.ape = ape;
		o.inited = true;

        $('head').append('<link rel="stylesheet" href="http://css.bmoon.com/mchat.css" />');
        $('body').append(html);
        if ($.browser.msie && $.browser.version=="6.0") $('#bchat').css('position','absolute');

		$('#bchat-trigger').toggle(o.openChat, o.closeChat);
        $('#bchat-input').bind('keydown', 'ctrl+return', o.msgSend);
		$('#bchat-snd').click(o.msgSend);
		return o;
	},

	openChat: function() {
		var o = bmoon.chat.init();

		$('#bchat-body').fadeIn();
        $('#bchat-input').focus();
	},

	closeChat: function() {
		var o = bmoon.chat.init();

		$('#bchat-body').fadeOut();
	},

	msgSend: function() {
		var o = bmoon.chat.init(),
		m = $('#bchat-input').val(),
		pipe = o.ape.lcsCurrentPipe;

		$('#bchat-input').val('');

		if (!m.length) return;

		if (o.adminuser.aname && pipe) {
			pipe.request.send("LCS_SEND", {msg: m});
		} else {
			o.ape.request.send("LCS_MSG", {uname: o.ape.lcsaname, msg: m});
		}
	},

	adminOn: function(data) {
		var o = bmoon.chat.init();

 		o.debug(data.pname + " 的管理员 " +data.aname + " 上线了");
		o.adminuser = data;
	},

	adminOff: function() {
		var o = bmoon.chat.init();

 		o.debug("管理员走开了");
		o.adminuser = {};
	}
};
