; var bmoon = bmoon || {};
bmoon.omsusers = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.omsusers;
		if (o.inited) return o;

		o.addoverlay = $('a[rel="#addoverlay"]').overlay({api: true});

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.omsusers.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.omsusers.init();

		$('#submit').unbind('click').click(o.userAdd);
	},
	
	userAdd: function() {
		if (!$('.VAL_NEWAPP').inputval()) return;

		$('#asn').attr('value', $.md5($.md5($('#asn').val())));

		var
		aname = $('#aname').val(),
		email = $('#email').val(),
		asn = $('#asn').val();

		$.post('/oms/users', {op: 'add', aname: aname, email: email, asn: asn}, function(data) {
			if (data.success != 1 || !data.aname) {
				alert(data.errmsg || '操作失败， 请稍后再试');
				return;
			}
		});
	}
};

$(document).ready(bmoon.omsusers.onready);
