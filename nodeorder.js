// $Id$

// Given a string like:
//
//   node-sort-list-3[]=node-sort-list-3_8&node-sort-list-3[]=node-sort-list-3_5&node-sort-list-3[]=node-sort-list-3_6
//
// and an id of:
//
//   node-sort-list-3
//
// return a string like:
//
//   8,5,6
//
function nodeorder_fix_values(a, id) {
  prefix = id + '_';
  prefix_len = prefix.length;
  s = new String;
  j = new String('');
  parts = a.split('&');
  for (i = 0; i < parts.length; i++) {
    u = parts[i].split('=');
    s += j;
    j = ',';
    s += u[1].slice(prefix_len);
  }
  return s;
}
