$(document).ready(
function(){
	$("a[rel=#addoverlay]").overlay({
		api: true,
		closeOnClick: false,
		onBeforeLoad: function() {
			var wrap = this.getContent().find("div.wrap");
			if (wrap.is(":empty")) {
				wrap.load(this.getTrigger().attr("href"));
			}
		}
	});

	$(".delprd").click(
	function(){
		if (confirm("确认删除该产品？")) {
			var id = $(this).attr("idval");
			$.getJSON("/cgi/prd", {id: id, op: "del"},
					 function(data) {
						 if (data.success == 1) {
							 alert("成功删除");
							 window.location.reload();
						 } else {
							 alert("删除失败， 请稍后再试");
						 }
					 }
			);
		}
	});
});
