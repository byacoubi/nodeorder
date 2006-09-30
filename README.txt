*******************************************************
    README.txt for nodeorder.module for Drupal
*******************************************************

This module was developed Funny Monkey to give users an easy way to
order nodes within their taxonomy terms.

By default, the taxonomy module orders listings of nodes by stickiness
and then by node creation date -- most recently posted nodes come first.

The nodeorder module gives the user the ability to put nodes in any
order they wish within each category that the node lives.

INSTALLATION:

Put the nodeorder directory in your modules directory.
Enable it via admin/modules.

CONFIGURATION:

You may turn on "orderability" on a per-vocabulary basis by visiting
your vocabularies' administration pages (admin/taxonomy).  This module
adds a checkbox on the "edit vocabulary" page titled "Orderable" -- it
defaults to being unchecked.  After checking this box and saving your
changes, you'll be able to order nodes that are classified in this
category.

You will also need to visit admin/access to give any roles rights to
"order nodes within categories".

USAGE:

Users with the ability to "order nodes within categories" will see a
link attached to every node that is classified in an orderable
vocabulary.  There is a "move up" and a "move down" link for each
term in the vocabulary that the node lives in.

TECHNICAL NOTES:

Upon installation, this module adds a new column (weight_in_tid) to the
term_node table.  Adding a column to a core table?  Are you crazy?  Yeah,
I guess so ... but it lets us keep the module's code very small since
most everything works through taxonomy.  Also it helps to avoid an extra
join for every node listing.

Please note that the node order is only respected when visiting links
that begin with "nodeorder" -- if you visit links that begin with
"taxonomy" they will appear in the generic taxonomy order.  Since the
module implements hook_term_path, the taxonomy links that get printed
per node will correctly point to the "nodeorder" space when they are in
orderable vocabularies.

TO DO:

1. Need a better UI for moving nodes within their categories -- AJAX...
