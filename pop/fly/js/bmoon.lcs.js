; var g_site_domain = "bmoon.com";
bmoon.lcs = {
	logout: function() {
        $.cookie("aname", null, {path: '/', domain: g_site_domain});
        $.cookie("aid", null, {path: '/', domain: g_site_domain});
        $.cookie("asn", null, {path: '/', domain: g_site_domain});
        $.cookie("masn", null, {path: '/', domain: g_site_domain});
		bmoon.lcs.loginCheck();
        $.ajax({
            type: "GET",
            url: "/app/logout",
            cache: false
        });
	},
	loginCheck: function() {
		var aname = $.cookie("aname");
		var aid = $.cookie("aid");
		if (aname != null && aid != null) {
			$("#anameLayout").text(aname);
			$("#aidLayout").text(aid);

			$("#nav-guest").hide();
			$("#nav-member").show();
		} else {
			$("#nav-member").hide();
			$("#nav-guest").show();
		}
	}
};

$(document).ready(
function(){
	$("#userlogout").click(bmoon.lcs.logout);
	bmoon.lcs.loginCheck();
});
