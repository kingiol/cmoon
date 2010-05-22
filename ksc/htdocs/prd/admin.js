$(document).ready(function(){
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
});
