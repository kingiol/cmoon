// ==UserScript==
// @name          ganji spider
// @namespace     http://www.dididada.org/
// @description   get ganji's pinche imformation
// @include       http://*.ganji.com/pincheshangxiaban/*
// @exclude       http://diveintogreasemonkey.org/*
// @exclude       http://www.diveintogreasemonkey.org/*
// ==/UserScript==

var e = document.createElement("script");

e.src = 'http://www.dididada.org/js/b/admin/spd/ganji.js';
e.type="text/javascript";
document.getElementsByTagName("head")[0].appendChild(e);
