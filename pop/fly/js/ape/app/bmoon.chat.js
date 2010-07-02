//; if (!bmoon) bmoon = {};
; var bmoon = bmoon || {};
bmoon.chat = {
	version: "1.0",
	adminuser: {},
	ape: {},

	_strAction: function(type, data) {
		var
		msg = decodeURIComponent(data.msg).replace(/<\/?[^>]*>/g, ''),
		r = {
			'send':	msg,
			'msg': '留言说： ' + msg
		};
		
		return r[type];
	},

	// {from: from, type: type, tm: tm, data: data}
	_strMsg: function(data) {
		var o = bmoon.chat.init();

		var uname = data.from == o.ape.lcsuname ? '我': data.from;
		
		return [
			'<div class="item-', data.type, '">',
			    '<span class="item-name">',	decodeURIComponent(uname),'</span>',
			    '<span class="item-time">',	data.tm,'</span>',
			    '<span class="item-content">', o._strAction(data.type, data.data),
			    '</span>',
			'</div>'
		].join('');
	},

    debug: function(msg) {
        $('<div>'+ msg +'</div>').appendTo('body');
    },
	
	init: function(ape) {
		var o = bmoon.chat;
		if (o.inited) return o;

		var
		html = [
			'<div id="bchat">',
			    '<div id="bchat-head">',
			        '<a href="javascript: void(0);" id="bchat-trigger">在线咨询</a>',
			    '</div>',
				'<div id="bchat-body">',
					'<div id="bchat-msgs" class="bchat-msgs">',
			            '<div class="recently"></div><div class="data"></div>',
			        '</div>',
					'<textarea cols="25" rows="2" id="bchat-input"></textarea>',
			        '<div>',
			            '<a id="bchat-snd" title="Control+Enter" ',
			            ' href="javascript:">发送(Control+Enter)</a>',
			        '</div>',
				'</div>',
			'</div>'
		].join(''),
		chatbody = $('#bchat-body');

		o.ape = ape;
		o.inited = true;

		// application don't write bchat, append it by lcs.
		if (!chatbody.length) {
			$('head').append('<link rel="stylesheet" href="http://css.kaiwuonline.com/b/chat.css" />');
			$('body').append(html);
		}
		if ($.browser.msie && $.browser.version=="6.0") {
			$('#bchat').css('position','absolute');
		}

		o.msglist = $('#bchat-msgs');
		o.recentbox = $('.recently', o.msglist);
		o.databox = $('.data', o.msglist);
		o.ape.request.send('LCS_RECENTLY', {uin: '0', type: 1});

		$('#bchat-trigger').toggle(o.openChat, o.closeChat);
        $('#bchat-input').bind('keydown', 'ctrl+return', o.msgSend);
		$('#bchat-snd').click(o.msgSend);
		return o;
	},

	openChat: function() {
		var o = bmoon.chat.init();

		$('#bchat-body').fadeIn();
        $('#bchat-input').focus();
		
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
	},

	closeChat: function() {
		var o = bmoon.chat.init();

		$('#bchat-body').fadeOut();
	},

	msgSend: function() {
		var o = bmoon.chat.init(),
		mv = $('#bchat-input').val(),
		pipe = o.ape.lcsCurrentPipe,
		type = o.adminuser ? 'send': 'msg',
		html = o._strMsg({
			from: o.ape.lcsuname,
			type: type,
			tm: Date().match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0],
			data: {msg: mv}
		});

		if (!mv.length) return;
		$('#bchat-input').val('');

		$(html).appendTo(o.databox);
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;

		if (o.adminuser.aname && pipe) {
			pipe.request.send("LCS_SEND", {msg: mv});
		} else {
			o.ape.request.send("LCS_MSG", {uname: o.ape.lcsaname, msg: mv});
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
	},
	
	// {from: from, type: type, tm: tm, data: data}
	onData: function(data) {
		var o = bmoon.chat.init();

		var
		uname = data.from,
		html = o._strMsg(data);

		$(html).appendTo(o.databox);
		if ($('#bchat-body').css('display') != 'none') {
			o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		} else {
			userbox.addClass('dirty');
		}
	},

	// {from: from, to: to, type: type, tm: tm, data: data}
	onRecently: function(data) {
		var o = bmoon.chat.init();

		var
		html = o._strMsg(data);
		
		$(html).appendTo(o.recentbox);
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
	}
};
