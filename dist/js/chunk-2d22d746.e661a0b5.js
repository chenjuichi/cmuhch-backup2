(window["webpackJsonp"]=window["webpackJsonp"]||[]).push([["chunk-2d22d746"],{f820:function(t,o,e){"use strict";e.r(o);var a,n=function(){var t=this,o=t.$createElement;t._self._c;return t._m(0)},s=[function(){var t=this,o=t.$createElement,e=t._self._c||o;return e("div",{staticClass:"about"},[e("h1",[t._v("This is an about page")])])}],c=e("ade3"),l=(e("fb6a"),e("bc3a")),i=e.n(l),u=(a={components:{},data:function(){return{}},mounted:function(){},created:function(){console.log("About, created()");var t=window.location.host;t=t.slice(0,t.lastIndexOf(":")),console.log("host: current ip:",t),this.url_host=t+":8060",this.host=t+":6060",this.protocol=window.location.protocol,i.a.defaults.baseURL=this.protocol+"//"+this.host,i.a.defaults.headers.common["X-Requested-With"]="XMLHttpRequest",console.log("axios base data: ",this.protocol+"//"+this.host),this.getSelectData()}},Object(c["a"])(a,"mounted",(function(){console.log("About, mounted()")})),Object(c["a"])(a,"computed",{}),Object(c["a"])(a,"methods",{getSelectData:function(){var t="/api/list-select";console.log("Axios get data test...",t),i.a.get(t).then((function(t){console.log("GET ok, Area table total records:",t.data.status)})).catch((function(t){console.error(t)}))}}),a),r=u,d=e("2877"),h=Object(d["a"])(r,n,s,!1,null,null,null);o["default"]=h.exports}}]);
//# sourceMappingURL=chunk-2d22d746.e661a0b5.js.map