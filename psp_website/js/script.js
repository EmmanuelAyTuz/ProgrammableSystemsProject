/* 
  Sistemas programables
  Domotica
  ITSVA
*/
var st = localStorage;
var btns = 4;

function sendValue(tagA, tagInput, uri) {
  // Con un boton
  document.getElementById(tagA).href =
    uri + document.getElementById(tagInput).value;
}

function saveBtn(i) {
  for (let index = 0; index < btns; index++) {
    if (i == "btnon" + index) {
      st.setItem("btn" + index, i);
    }
    if (i == "btnoff" + index) {
      st.setItem("btn" + index, i);
    }
  }
}

function getVal() {
  // Guardar valor en carga de la pagina
  for (let index = 0; index < btns; index++) {
    if (st.getItem("btn" + index) == null) {
      st.setItem("btn" + index, "OK");
      console.log(st.getItem("btn" + index));
    }
  }
  var cc;
  //console.log("BTN" + i + ": " + st.getItem("btn" + i));
  for (let index = 0; index < btns; index++) {
    if (st.getItem("btn" + index) != "OK") {
      console.log("BTN" + index + ": " + st.getItem("btn" + index));
      cc = document.getElementById(st.getItem("btn" + index));
      cc.className = cc.className.replace(
        /\bbtn-stylist-green\b/g,
        "btn-stylist-click-green"
      );
      cc = document.getElementById(st.getItem("btn" + index));
      cc.className = cc.className.replace(
        /\bbtn-stylist-red\b/g,
        "btn-stylist-click-red"
      );
    }
  }

  //Vars
  var venti = location.search.split("?venti=")[1];
  var light = location.search.split("?light=")[1];

  var ovn = document.getElementById("rangeVent");
  var ivn = document.getElementById("ventilator");

  var oi = document.getElementById("rangeLight");
  var ii = document.getElementById("light");

  if (venti != undefined && light == undefined) {
    st.setItem("venti", venti);
    ovn.value = st.getItem("venti");
    ivn.value = st.getItem("venti");

    oi.value = st.getItem("light");
    ii.value = st.getItem("light");
  }
  if (venti == undefined && light != undefined) {
    ovn.value = st.getItem("venti");
    ivn.value = st.getItem("venti");
    st.setItem("light", light);
    oi.value = st.getItem("light");
    ii.value = st.getItem("light");
  }
  if (venti == undefined) {
    ovn.value = st.getItem("venti");
    ivn.value = st.getItem("venti");
    if (st.getItem("venti") == null) {
      ovn.value = 0;
      ivn.value = 0;
    }
  }
  if (light == undefined) {
    oi.value = st.getItem("light");
    ii.value = st.getItem("light");
    if (st.getItem("light") == null) {
      oi.value = 0;
      ii.value = 0;
    }
  }
  console.log("Local Venti: " + st.getItem("venti"));
  console.log("Local Light: " + st.getItem("light"));
  console.log("Valor venti URL: " + venti);
  console.log("Out range: " + ovn.value);
  console.log("In range: " + ivn.value);
  console.log("Valor light URL: " + light);
  console.log("Out range: " + oi.value);
  console.log("In range: " + ii.value);
}
