; var bmoon = bmoon || {};
bmoon.memberpass = {
    version: '1.0',
    
    init: function() {
        var o = bmoon.memberpass;
        if (o.inited) return o;

        o.msn = $('#msn');
        o.msnr = $('#msn-r');
        o.submit = $('#submit');

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.memberpass.init();

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.memberpass.init();

        o.msnr.blur(o.msnCheck);
        o.submit.click(o.memberPass);
    },

    msnCheck: function() {
        var o = bmoon.memberpass.init();
        
        if (!$('.VAL_MEMBERPASS').inputval()) return false;
        
        var msn = o.msn.val(),
        msnr = o.msnr.val(),
        p = $(this).parent();

        $(".vres", p).remove();
        p.removeClass('success').removeClass('error');
        if (msn !== msnr) {
            $('<span class="vres">两次密码不匹配</span>').appendTo(p);
            p.addClass('error');
            return false;
        } else {
            p.addClass('success');
        }
        return true;
    },

    memberPass: function() {
        var o = bmoon.memberpass.init();

        if (!$('.VAL_MEMBERPASS').inputval()) return;
        if (!o.msnCheck()) return;

        var msn = $.md5($.md5(o.msn.val())),
        p = $(this).parent();

        $(".vres", p).remove();
        p.removeClass('success').removeClass('error').addClass('loading');
        $.post('/json/member/edit', {_op: 'mod', msn: msn}, function(data) {
            p.removeClass('loading');
            if (data.success == 1) {
                p.addClass('success');
            } else {
                p.addClass('error');
                $('<span class="vres">'+ data.errmsg +'</span>').appendTo(p);
            }
        }, 'json');
    }
};

$(document).ready(bmoon.memberpass.onready);
