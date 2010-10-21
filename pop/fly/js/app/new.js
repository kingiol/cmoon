; var bmoon = bmoon || {};
bmoon.appnew = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.appnew;
		if (o.inited) return o;

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.appnew.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.appnew.init();

		$('#aname').blur(o.appCheck);
		$('#submit').click(o.appAdd);
	},

	appCheck: function() {
		var o = bmoon.appnew.init();
		
		var
		aname = $('#aname').val(),
		p = $(this).parent();
		
		if (aname.length > 0) {
			$(".error", p).remove();
			p.removeClass('success').removeClass('error').addClass('loading');
			$.getJSON('/json/app/exist', {aname: aname}, function(data) {
				if (data.success == 1) {
					if (data.exist == 1) {
						$('<span class="error">'+ data.msg +'</span>').appendTo(p);
						p.addClass('error');
					} else {
						p.addClass('success');
					}
				} else {
					p.addClass('error');
					$('<span class="error">'+ data.errmsg +'</span>').appendTo(p);
				}
			});
		}
	},

	appAdd: function() {
		var o = bmoon.appnew.init();

		if (!$('.VAL_NEWAPP').inputval()) return;

		$('#asn').attr('value', $.md5($.md5($('#asn').val())));

		var
		aname = $('#aname').val(),
		email = $('#email').val(),
		asn = $('#asn').val();

		$.getJSON('/json/app/new', {aname: aname, email: email, asn: asn}, function(data) {
			if (data.success != 1 || !data.aname) {
				alert(data.errmsg || '操作失败， 请稍后再试');
				return;
			}
			$('.copy-aname').text(data.aname);
			$('#add').fadeOut();
			$('#copy').fadeIn();
			
			bmoon.lcs.loginCheck();
		});
	}
};

$(document).ready(bmoon.appnew.onready);
