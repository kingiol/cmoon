; if (!bmoon.lcs) bmoon.lcs = {};
bmoon.lcs.chat = {
	inited: false,
	ape: {},

	init: function(ape) {
		var o = bmoon.lcs.chat,
		html = [
			'<div id="bchat">',
				'<div id="bchat-head"><a href="javascript: void(0);" id="bchat-trigger">留言</a></div>',
				'<div id="bchat-body"></div>',
			'</div>'
		].join('');
		if (o.inited) return;

        $('head').append('<link rel="stylesheet" href="http://css.bmoon.com/mchat.css" />');
        $('body').append(html);
        if ($.browser.msie && $.browser.version=="6.0") $('#bchat').css('position','absolute');

		o.ape = ape;

		o.inited = 1;
	}
};
