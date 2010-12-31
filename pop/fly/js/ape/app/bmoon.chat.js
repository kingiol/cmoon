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

	_blinkToggle: function() {
		var o = bmoon.chat.init();

		if (!o.toggle.blinkID) {
			o.toggle.blinkID = setInterval(function() {
				o.toggle.toggleClass('dirty');
			}, 500);
		}
	},

	_stopBlinkToggle: function() {
		var o = bmoon.chat.init();

		clearInterval(o.toggle.blinkID);
		o.toggle.removeClass('dirty');
		o.toggle.blinkID = 0;
	},

	_rendBox: function() {
		var o = bmoon.chat.init();

		o.ielow && o.rendo.animate({
			top: $(window).scrollTop() + ($(window).height() - o.rendo.height()),
			right: '20px'
		}, 10);
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
				'<div id="kol-lcs-min" title="打开">&nbsp;</div>',
				'<div id="kol-lcs-max">',
					'<div id="kol-lcs-body">',
			    	    '<div class="top">客服当前离线，留言功能开启。</div>',
						'<div class="close" title="最小化">&nbsp;</div>',
						'<div class="msgs">',
			    	        '<div class="recently"></div><div class="data"></div>',
			    	    '</div>',
						'<textarea rows="2" id="kol-lcs-input"></textarea>',
						'<div class="bot">Enter 发送消息。 ',
							'<span><input type="checkbox" id="kol-lcs-remind-sw" checked="checked" /> 提示音</span>',
						'</div>',
					'</div>',
			    	'<div id="kol-lcs-head">',
						'<div class="toggle" title="在线聊天">&nbsp;</div>',
						'<div class="miner" title="关闭">&nbsp;</div>',
			    	'</div>',
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
		o.min = $('#kol-lcs-min');
		o.max = $('#kol-lcs-max');
		o.rendo = o.max;
		o.head = $('#kol-lcs-head');
		o.body = $('#kol-lcs-body');
		
		o.toggle = $('.toggle', o.head);
		o.closer = $('.close', o.body);
		o.miner = $('.miner', o.head);
		
		o.msglist = $('.msgs', o.body);
		o.recentbox = $('.recently', o.msglist);
		o.databox = $('.data', o.msglist);
		o.hint = $('.top', o.body);
		
		o.reminder = $('#kol-lcs-remind')[0];
		o.rmdsw = $('#kol-lcs-remind-sw');
		o.msginput = $('#kol-lcs-input');

		o.ielow = bmoon.utl.ie() && bmoon.utl.ie() < 7;

		o.initUI();
		
		return o;
	},

	initUI: function() {
		var o = bmoon.chat.init();

		var uimax = $.cookie('lcs_ui_max'),
		attr = {
			position: 'absolute',
			top: $(window).height() + $(window).scrollTop(),
			right: '20px'
		};

		if ( (!o.ape.restoreUI && o.ape.defaultUI == 'min') || (o.ape.restoreUI && ((uimax == null && o.ape.defaultUI == 'min') || uimax == '0' )) )
			o.rendo = o.min;

		// ielow double margin on float node. set display inline to fix it
		o.ielow && o.toggle.css('display', 'inline');
		attr.top -= o.rendo.height();
		// ielow position: fixed bug. set absolute and scrool to fix it
		if (o.ielow) {
			o.rendo.css(attr);
			$(window).scroll(o._rendBox);
			$(window).resize(o._rendBox);
		}
		
		o.rendo.fadeIn();
		
		o.miner.click(function() {
			o.max.hide();
			o.min.fadeIn();
			$.cookie('lcs_ui_max', "0");
		});
		o.min.click(function() {
			o.min.hide();
			o.max.fadeIn();
			$.cookie('lcs_ui_max', "1");
		});
		
		o.closer.click(o.closeChat);
		o.toggle.click(o.openChat);
		o.msginput.bind('keydown', 'return', o.msgSend);
	},
	
	openChat: function() {
		var o = bmoon.chat.init();

		o.body.fadeIn();
        o.msginput.focus();
		
		o.msglist[0].scrollTop = o.msglist[0].scrollHeight;
		o._stopBlinkToggle();
		o.toggle.unbind('click').click(o.closeChat);
	},

	closeChat: function() {
		var o = bmoon.chat.init();

		o.body.fadeOut();
		o.toggle.unbind('click').click(o.openChat);
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
			o._blinkToggle();
		}

		o.soundRemind('receive');
	},

	// {from: from, to: to, type: type, tm: tm, data: data}
	onRecently: function(data) {
		var o = bmoon.chat.init();

		var
		html = o._strMsg(data);
		
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
