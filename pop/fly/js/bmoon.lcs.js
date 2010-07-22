; var g_site_domain = "kaiwuonline.com";
var bmoon = bmoon || {};
bmoon.lcs = {
	version: "1.0",

	init: function() {
		var o = bmoon.lcs;

		if (o.inited) return o;
		o.inited = true;

		o.aname = $('#anameLayout');
		o.nav_guest = $('#nav-guest');
		o.nav_member = $('#nav-member');
		o.login_aname = $('#login-aname');
		o.login_asn = $('#login-asn');
		o.loginoverlay = $('a[rel="#loginoverlay"]').overlay({mask: '#666', api: true});
		
		return o;
	},

	onready: function() {
		var o = bmoon.lcs.init();

		o.bindClick();
		o.loginCheck();

		//$('ul.sf-menu').superfish();
		
		o.loginref = bmoon.utl.getQueryString("loginref");
		if (o.loginref) {
			o.loginoverlay.load();
		}
		o.vikierr = bmoon.utl.getQueryString("vikierr");
		if (o.vikierr) {
			$('#content').empty().append('<div class="error">'+o.vikierr+'</div>')
		}
	},
	
	bindClick: function() {
		var o = bmoon.lcs.init();
		
		$('#login-submit').click(o.login);
		$('#userlogout').click(o.logout);
	},

	login: function() {
		var o = bmoon.lcs.init();
		
		if (!$(".VAL_LOGIN").inputval()) return;

		var
		aname = o.login_aname.val(),
		asn = $.md5($.md5(o.login_asn.val()));

		$.getJSON("/json/app/login", {aname: aname, asn: asn}, function(data) {
			if (data.success != 1 || !data.aname) {
				alert(data.errmsg || "操作失败， 请稍后再试");
				return;
			}
			o.loginoverlay.close();
			setTimeout(function() {location.href = o.loginref || location.href;}, 1000);
		});
	},
	
	logout: function() {
		var o = bmoon.lcs.init();
		
        $.cookie('aname', null, {path: '/', domain: g_site_domain});
        $.cookie('asn', null, {path: '/', domain: g_site_domain});
        $.cookie('masn', null, {path: '/', domain: g_site_domain});
		o.loginCheck();
		
        $.ajax({
            type: 'GET',
            url: '/json/app/logout',
            cache: false
        });
	},
	
	loginCheck: function() {
		var o = bmoon.lcs.init();
		
		var aname = $.cookie('aname');
		if (aname != null) {
			o.aname.text(aname);
			o.nav_guest.hide();
			o.nav_member.show();
		} else {
			o.nav_member.hide();
			o.nav_guest.show();
		}
	}
};

$(document).ready(bmoon.lcs.onready);
