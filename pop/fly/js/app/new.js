; var bmoon = bmoon || {};
bmoon.appnew = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.appnew;
		if (o.inited) return o;

		o.aname = $('#aname');
		o.email = $('#email');
		o.asn = $('#asn');
		o.submit = $('#submit');

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.appnew.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.appnew.init();

		o.aname.blur(o.appCheck);
		o.submit.click(o.appAdd);
	},

	appCheck: function() {
		var o = bmoon.appnew.init();
		
		var
		aname = o.aname.val(),
		p = $(this).parent();
		
		if (aname.length > 0) {
			$('.vres', p).remove();
			p.removeClass('success').removeClass('error').addClass('loading');
			$.getJSON('/json/app/exist', {aname: aname}, function(data) {
				p.removeClass('loading');
				if (data.success == 1) {
					if (data.exist == 1) {
						$('<span class="vres">'+ data.msg +'</span>').appendTo(p);
						p.addClass('error');
					} else {
						p.addClass('success');
					}
				} else {
					p.addClass('error');
					$('<span class="vres">'+ data.errmsg +'</span>').appendTo(p);
				}
			});
		}
	},

	appAdd: function() {
		var o = bmoon.appnew.init();

		if (!$('.VAL_NEWAPP').inputval()) return;

		o.asn.attr('value', $.md5($.md5(o.asn.val())));

		var	aname = o.aname.val(),
		email = o.email.val(),
		asn = o.asn.val();

		$.post('/json/app/new', {_op: 'add', aname: aname, email: email, asn: asn}, function(data) {
			if (data.success != 1 || !data.aname) {
				alert(data.errmsg || '操作失败， 请稍后再试');
				return;
			}
			$('.copy-aname').text(data.aname);
			$('#add').fadeOut();
			$('#copy').fadeIn();
			
			bmoon.lcs.loginCheck();
		}, 'json');
	}
};

$(document).ready(bmoon.appnew.onready);
