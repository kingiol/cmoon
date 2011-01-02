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

	_blink: function() {
		var o = bmoon.chat.init();

		if (!o.head.blinkID) {
			o.head.blinkID = setInterval(function() {
				o.head.toggleClass('dirty');
			}, 500);
		}
	},

	_stopBlink: function() {
		var o = bmoon.chat.init();

		clearInterval(o.head.blinkID);
		o.head.removeClass('dirty');
		o.head.blinkID = 0;
	},

	_rendBox: function(opts) {
		var o = bmoon.chat.init();

		if (o.ielow) {
			opts = $.extend({
				ani: true
			}, opts || {});
			o.rendo.css('position', 'absolute');
			if (opts.ani) {
				o.rendo.animate({
					top: $(window).scrollTop() + ($(window).height() - o.rendo.height()),
					right: o.pos.right
				}, 10);
			} else {
				o.rendo.css({
					top: $(window).scrollTop() + ($(window).height() - o.rendo.height()),
					right: o.pos.right
				});
			}
		}
	},

    debug: function(msg) {
        //$('<div>'+ msg +'</div>').appendTo('body');
    },
	
	init: function(ape) {
		var o = bmoon.chat;
		if (o.inited) return o;
		o.inited = true;

		var	rmdhtml = $.browser.msie? '<bgsound id="kol-lcs-remind"></bgsound>': '<audio id="kol-lcs-remind"></audio>',
		html = [
			'<div id="kol-lcs">',
			    '<div id="kol-lcs-head">&nbsp;</div>',
				'<div id="kol-lcs-body">',
			        '<div class="top">',
						'<div class="close" title="最小化">&nbsp;</div>',
						'<span class="hint">客服当前离线，留言功能开启。</span>',
					'</div>',
					'<div class="msgs">',
			            '<div class="recently"></div><div class="data"></div>',
			        '</div>',
					'<div class="bot">输入消息：',
						'<span><input type="checkbox" id="kol-lcs-remind-sw" checked="checked" /> 提示音</span>',
					'</div>',
					'<textarea id="kol-lcs-input"></textarea>',
				'</div>',
				rmdhtml,
			'</div>'
		].join('');
		
		o.chatbody = $('#kol-lcs');

		// application don't write kol-lcs, append it.
		if (!o.chatbody.length) {
			$('body').append(html);
		}

		o.ape = ape;
		o.head = $('#kol-lcs-head');
		o.body = $('#kol-lcs-body');
		o.closer = $('.close', o.body);
		o.rendo = o.head;
		
		o.msglist = $('.msgs', o.body);
		o.recentbox = $('.recently', o.msglist);
		o.databox = $('.data', o.msglist);
		o.hint = $('.hint', o.body);
		
		o.reminder = $('#kol-lcs-remind')[0];
		o.rmdsw = $('#kol-lcs-remind-sw');
		o.msginput = $('#kol-lcs-input');

		o.ielow = bmoon.utl.ie() && bmoon.utl.ie() < 7;

		o.pos = {
			right: o.chatbody.css('right') ? o.chatbody.css('right'): 0,
			bottom: o.chatbody.css('bottom') ? o.chatbody.css('bottom'): 0
		};
		o.initUI();
		
		return o;
	},

	initUI: function() {
		var o = bmoon.chat.init();

		if (o.ielow) {
			// ielow double margin on float node. set display inline to fix it
			o.hint.css('display', 'inline');
			o.closer.css('display', 'inline');

			// ielow position: fixed bug. set absolute and scrool to fix it
			o._rendBox({ani: false});
			$(window).scroll(o._rendBox);
			$(window).resize(o._rendBox);
		}
		
		o.rendo.fadeIn();
		
		o.head.mouseover(o.openChat);
		o.closer.click(o.closeChat);
		o.msginput.bind('keydown', 'return', o.msgSend);
	},
	
	openChat: function() {
		var o = bmoon.chat.init();

		o.head.hide();
		o.body.fadeIn();
		o.rendo = o.body;
		o._rendBox({ani: false});
        o.msginput.focus();
		
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		o._stopBlink();
	},

	closeChat: function() {
		var o = bmoon.chat.init();

		o.body.hide();
		o.head.show();
		o.rendo = o.head;
		o._rendBox({ani: false});
	},

	msgSend: function() {
		var o = bmoon.chat.init();
		
		var mv = o.msginput.val(),
		pipe = o.ape.lcsCurrentPipe,
		type = o.adminuser ? 'send': 'msg',
		html = o._strMsg({
			from: o.ape.lcsuname,
			type: type,
			tm: Date().match(/\d{1,2}:\d{1,2}:\d{1,2}/)[0],
			data: {msg: mv}
		});

		if (!mv.length) return false;
		if (mv.length > 256) {
			var ot = o.hint.html();
			o.hint.html('聊天信息不要超过 256 个字。');
			setTimeout(function(){o.hint.html(ot);}, 2000);
			return false;
		}
		o.msginput.val('');

		$(html).appendTo(o.databox);
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;

		if (o.adminuser.aname && pipe) {
			pipe.request.send('LCS_SEND', {msg: mv});
		} else {
			o.ape.request.send('LCS_MSG', {uname: o.ape.lcsaname, msg: mv});
		}
		
		o.soundRemind('send');
		return false;
	},

	adminOn: function(data) {
		var o = bmoon.chat.init();

 		o.debug(data.pname + ' 的管理员 ' +data.aname + ' 上线了');
		o.hint.html(data.aname + ' 当前在线。');
		o.adminuser = data;
		
		o.soundRemind('login');
	},

	adminOff: function() {
		var o = bmoon.chat.init();

 		o.debug('管理员走开了');
		o.hint.html('客服当前离线，留言功能开启。');
		o.adminuser = {};
	},
	
	// {from: from, type: type, tm: tm, data: data}
	onData: function(data) {
		var o = bmoon.chat.init();

		var
		uname = data.from,
		html = o._strMsg(data);

		$(html).appendTo(o.databox);
		if (o.body.css('display') != 'none') {
			o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		} else {
			o._blink();
		}

		o.soundRemind('receive');
	},

	// {from: from, to: to, type: type, tm: tm, data: data}
	onRecently: function(data) {
		var o = bmoon.chat.init();

		var html = o._strMsg(data);
		
		$(html).prependTo(o.recentbox);
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
	},

	soundRemind: function(type) {
		var o = bmoon.chat.init();

		if (o.rmdsw.attr('checked') == true) {
			o.reminder.src = 'http://www.kaiwuonline.com/obj/audio/'+type+'.wav';
			if (!$.browser.msie) {
				o.reminder.load();
				o.reminder.play();
			}
		}
	}
};
