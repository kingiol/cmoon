; var bmoon = bmoon || {};
bmoon.omsstat = {
	version: '1.0',

	_showTooltip: function(x, y, contents) {
		$('<div id="tooltip">' + contents + '</div>').css( {
			position: 'absolute',
			display: 'none',
			top: y + 5,
			left: x + 5,
			border: '1px solid #fdd',
			padding: '2px',
			'background-color': '#fee',
			opacity: 0.80
		}).appendTo("body").fadeIn(200);
	},
		
	init: function() {
		var o = bmoon.omsstat;
		if (o.inited) return o;

		o.sttype = $('select[name=stat-type]');
		o.stres = $('#st-res');
		o.stdata = mgd.stdata;

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.omsstat.init();

		o.bindClick();

		bmoon.utl.after(o.rendStat, 'mgd.stdata !== undefined', 10);
	},

	bindClick: function() {
		var o = bmoon.omsstat.init();

		$('.datep').datePicker({startDate: '2010-10-10'});
		o.stres.bind('plothover', o.plotOver);
		o.sttype.change(o.rendStat);
	},

	rendStat: function() {
		var o = bmoon.omsstat.init();

		var type = o.sttype.val();

		o.stdata = o.stdata || mgd.stdata;
		
		if (type == 'visit') {
			$.plot(o.stres,
				   [
					   {label: '日访问量', data: o.stdata.pv},
					   {label: '日用户量', data: o.stdata.uv}
				   ],
				   {
					   xaxis: {mode: 'time', timeformat: "%y-%m-%d"},
					   series: {
						   lines: {show: true},
						   points: {show:true}
					   },
					   grid: {hoverable: true}
				   });
		} else {
			$.plot(o.stres, [o.stdata[type]],
				   {
					   series: {
						   bars: {show: true, barWidth: 0.6}
					   },
					   grid: {hoverable: true}
				   });
		}
	},

	plotOver: function(event, pos, item) {
		var o = bmoon.omsstat.init();

        if (item) {
            if (o.previousPoint != item.datapoint) {
                o.previousPoint = item.datapoint;
                
                $("#tooltip").remove();
				
                var	x = item.datapoint[0],
                y = item.datapoint[1];

				if (o.sttype.val() == 'visit') {
					var dt = new Date(x),
					Y = dt.getFullYear(), m = dt.getMonth()+1, d = dt.getDate(),
					tm = Y+"-"+m+"-"+d;
					
					o._showTooltip(item.pageX, item.pageY,
								   tm + ' ' + item.series.label + ' : ' + y);
				} else {
					o._showTooltip(item.pageX, item.pageY,
                                   o.stdata.aux[x] + ' ' + ' : ' + y);
				}
            }
        } else {
            $("#tooltip").remove();
            o.previousPoint = null;            
        }
	}
};

$(document).ready(bmoon.omsstat.onready);
