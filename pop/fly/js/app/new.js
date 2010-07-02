; var bmoon = bmoon || {};
bmoon.appnew = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.appnew;
		if (o.inited) return o;

		o.ht_exist = $('#hit-appexist');
		o.ht_nexist = $('#hit-appnexist'),

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.appnew.init();

		o.bindSubmit();
	},

	bindSubmit: function() {
		var o = bmoon.appnew.init();
		
		$("#aname").blur(function() {
			var aname = $("#aname").val();
			if (aname.length > 0) {
				o.ht_exist.fadeOut();
				o.ht_nexist.fadeOut();
				$.getJSON("/app/exist", {aname: aname}, function(data) {
					if (data.success == 1) {
						if (data.exist == 1) {
							o.ht_nexist.fadeOut();
							o.ht_exist.fadeIn('slow');
						} else {
							o.ht_exist.fadeOut();
							o.ht_nexist.fadeIn('slow');
						}
					}
				});
			}
		});

		$("#submit").click(function() {
			if (!$(".VAL_NEWAPP").inputval()) return;

			$("#asn").attr("value", $.md5($.md5($("#asn").val())));

			var
			aname = $("#aname").val(),
			email = $("#email").val(),
			asn = $("#asn").val();


			$.getJSON("/app/new", {aname: aname, email: email, asn: asn}, function(data) {
				if (data.success != 1 || !data.aname) {
					alert(data.errmsg || "操作失败， 请稍后再试");
					return;
				}
				$("#copy-aname").text(data.aname);
				$("#add").fadeOut();
				$("#copy").fadeIn();
				
				bmoon.lcs.loginCheck();
			});
		});
	}
};

$(document).ready(bmoon.appnew.onready);
