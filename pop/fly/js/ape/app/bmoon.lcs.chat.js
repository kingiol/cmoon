; if (!bmoon.lcs) bmoon.lcs = {};
bmoon.lcs.chat = {
	inited: false,
	ape: {},

	init: function(ape) {
		var o = bmoon.lcs.chat,
		html = [
			'<div id="bchat">',
				'<div id="bchat-head"><a href="javascript: void(0);" id="bchat-trigger">留言</a></div>',
				'<div id="bchat-body">',
					'<div id="bchat-msgs"></div>',
					'<textarea cols="34" rows="3" id="bchat-input"></textarea>',
					'<div><a title="Control+Enter" href="javascript:" id="bchat-snd">发送</a></div>',
				'</div>',
			'</div>'
		].join('');
		if (o.inited) return;

        $('head').append('<link rel="stylesheet" href="http://css.bmoon.com/mchat.css" />');
        $('body').append(html);
        if ($.browser.msie && $.browser.version=="6.0") $('#bchat').css('position','absolute');

		$('#bchat-trigger').toggle(o.openChat, o.closeChat);
		/*
		$('#bchat-trigger').click(
		function(){
			$('#bchat-body').toggle();
		});
		 */

		o.ape = ape;

		o.inited = 1;
	},

	openChat: function() {
		$('#bchat-body').fadeIn();
	},

	closeChat: function() {
		$('#bchat-body').fadeOut();
	}
};
