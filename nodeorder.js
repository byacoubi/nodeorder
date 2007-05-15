// $Id$

// Given a string like:
//
//   node-sort-list-3[]=node-sort-list-3_8_21&node-sort-list-3[]=node-sort-list-3_5_16&node-sort-list-3[]=node-sort-list-3_6_19
//
// and an id of:
//
//   node-sort-list-3
//
// return an array of two strings like:
//
//   '8,5,6','21,16,19'
//
// Where the first string represents the nids and the second string represents the vids...
//
function nodeorder_fix_values(a, id) {
  prefix = id + '_';
  prefix_len = prefix.length;
  sNids = new String;
  sVids = new String;
  j = new String('');
  parts = a.split('&');
  for (i = 0; i < parts.length; i++) {
    u = parts[i].split('=');
    sNids += j;
    sVids += j;
    j = ',';

    separatorIndex = u[1].lastIndexOf('_');
    sNids += u[1].slice(prefix_len, separatorIndex);
    sVids += u[1].slice(separatorIndex + 1);
  }
  
  a = new Array();
  a[0] = sNids;
  a[1] = sVids;
  
  return a;
}
