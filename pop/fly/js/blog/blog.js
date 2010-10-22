; var bmoon = bmoon || {};
bmoon.blog = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.blog;
		if (o.inited) return o;

		o.pid = 0;
		o.commentnum = 0;
		o.author = Cookie.read("lcs_uname");
		if (!o.author) o.author = '网友';
		o.comments = $('#comments');
		
		o.inited = true;
		return o;
	},

	// { "id": "1", "type": "0", "oid": "7", "pid": "0", "ip": "127.0.0.1", "addr": "CZ88.NET", "author": "ru_PGFOV", "content": "hey, man\n\n  Baby\n", "intime": "2010-10-22 16:53:42", "uptime": "2010-10-22 16:53:42" }
	_nodeComment: function(cmt) {
		var o = bmoon.blog.init();

		var r = $('#comment-'+cmd.id),
		html = [
			'<div class="comment-item" id="comment-', cmt.id, '">',
				'<pre class="box">',
					'<div class="content">', cmt.content, '</div>',
				'</pre>',
				'<div class="info">',
					'<span class="author">',
						'来自 ', cmt.addr, ' 的 ', cmt.author,
					'</span>',
					'<span class="date">', cmt.intime, '</span>',
				'</div>',
			'</div>'
		].join('');

		if (!r.length) {
			r = $(html);
		}
		return r;
	},
	
	onready: function() {
		var o = bmoon.blog.init();

		o.bindClick();
		o.getComment();
	},

	bindClick: function() {
		var o = bmoon.blog.init();

		$('#submit-comment-add').click(o.addComment);
	},

	getComment: function() {
		var o = bmoon.blog.init();

		$.getJSON('/json/comment', {ids: '0:'+o.bid}, function(data) {
			if (data.success == 1 && bmoon.utl.type(data[0]) == 'Object') {
				o.commentnum = data[0][o.bid].ntt;
				$('#comment-num-'+o.bid).text(o.commentnum);
				if (bmoon.utl.type(data[0][o.bid].cmts) == 'Object') {
					$.each(data[0][o.bid].cmts, function(k, v){
						if (v.pid == 0)
							o._nodeComment(v).appendTo(o.comments);
						else
							o._nodeComment(v).appendTo($('#comment-'+v.pid));
					});
				}
			}
		});
	},

	addComment: function() {
		var o = bmoon.blog.init();

		if (!$('.VAL_NEWCOMMENT').inputval()) return;

		var content = $('#b-comment').val(),
		p = $(this).parent();

		$(".error", p).remove();
		p.removeClass('success').removeClass('error').addClass('loading');

		$.post('/comment',
			   {
				   op: 'add', type: 0, oid: o.bid, pid: o.pid,
				   author: o.author,
				   content: content
			   },
			   function(data) {
				   p.removeClass('loading');
				   
				   if (data.success == 1) {
					   p.addClass('success');
					   o.commentnum++;
					   $('#comment-num-'+o.bid).text(o.commentnum);
					   $('#b-content').val("");
				   } else {
					   p.addClass('error');
					   $('<span class="error">'+ data.errmsg +'</span>').appendTo(p);
				   }
			   },
			   'json');
		
	}
};

$(document).ready(bmoon.blog.onready);
