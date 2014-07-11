<?php
/**
 * Menu callback; displays all nodes associated with a term.
 *
 * @param $term
 *   The taxonomy term.
 * @return
 *   The page content.
 */
function nodeorder_taxonomy_term_page($term) {
  // Assign the term name as the page title.
  drupal_set_title($term->name);

  // Build breadcrumb based on the hierarchy of the term.
  $current = (object) array(
    'tid' => $term->tid,
  );
  // @todo This overrides any other possible breadcrumb and is a pure hard-coded
  //   presumption. Make this behavior configurable per vocabulary or term.
  $breadcrumb = array();
  while ($parents = taxonomy_get_parents($current->tid)) {
    $current = array_shift($parents);
    $breadcrumb[] = l($current->name, 'taxonomy/term/' . $current->tid);
  }
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb = array_reverse($breadcrumb);
  drupal_set_breadcrumb($breadcrumb);
  drupal_add_feed('taxonomy/term/' . $term->tid . '/feed', 'RSS - ' . $term->name);

  $build = array();

  $build['term_heading'] = array(
    '#prefix' => '<div class="term-listing-heading">',
    '#suffix' => '</div>',
    'term' => taxonomy_term_view($term, 'full'),
  );

  if ($nids = taxonomy_select_nodes($term->tid, TRUE, variable_get('default_nodes_main', 10),
    array('t.weight' => 'ASC'))) {
    $nodes = node_load_multiple($nids);
    $build += node_view_multiple($nodes);
    $build['pager'] = array(
      '#theme' => 'pager',
      '#weight' => 5,
     );
  }
  else {
    $build['no_content'] = array(
      '#prefix' => '<p>',
      '#markup' => t('There is currently no content classified with this term.'),
      '#suffix' => '</p>',
    );
  }
  return $build;
}

/**
 * Menu callback; displays all nodes associated with a term.
 *
 * @todo: It seems this function is never called.
 */
function nodeorder_term_page($str_tids = '', $depth = 0, $op = 'page') {
  $terms = taxonomy_terms_parse_string($str_tids);
  if ($terms['operator'] != 'and' && $terms['operator'] != 'or') {
    drupal_not_found();
  }

  if ($terms['tids']) {
    // TODO Please convert this statement to the D7 database API syntax.
    $result = db_query(db_rewrite_sql('SELECT t.tid, t.name FROM {taxonomy_term_data} t WHERE t.tid IN (' . db_placeholders($terms['tids']) . ')', 't', 'tid'), $terms['tids']);
    $tids = array(); // we rebuild the $tids-array so it only contains terms the user has access to.
    $names = array();
    while ($term = db_fetch_object($result)) {
      $tids[] = $term->tid;
      $names[] = $term->name;
    }

    if ($names) {
      drupal_set_title($title = implode(', ', $names));

      // Set the order that gets passed in to taxonomy_select_nodes.
      // This probably breaks down when there's a query that spans
      // multiple terms...
      //
      // First sort by sticky, then by weight...
      if ($terms['operator'] == 'or') {
        $order = 'n.sticky DESC, tn.weight';
      }
      else {
        $order = 'n.sticky DESC, tn0.weight';
      }

      switch ($op) {
        case 'page':
          // Build breadcrumb based on first hierarchy of first term:
          $current->tid = $tids[0];
          $breadcrumb = array();
          while ($parents = taxonomy_get_parents($current->tid)) {
            $current = array_shift($parents);
            $breadcrumb[] = l($current->name, 'nodeorder/term/' . $current->tid);
          }
          $breadcrumb[] = l(t('Home'), NULL);
          $breadcrumb = array_reverse($breadcrumb);
          drupal_set_breadcrumb($breadcrumb);

          module_load_include('inc', 'taxonomy', 'taxonomy.pages'); //.inc files are not loaded automatically
          // TODO Please change this theme call to use an associative array for the $variables parameter.
          $output = theme('taxonomy_term_page', $tids, nodeorder_select_nodes($tids, $terms['operator'], $depth, TRUE, $order));

          drupal_add_feed(url('taxonomy/term/' . $str_tids . '/' . $depth . '/feed'), 'RSS - ' . $title);
          return $output;

        case 'feed':
          $channel['link'] = url('nodeorder/term/' . $str_tids . '/' . $depth, array('absolute' => TRUE));
          $channel['title'] = variable_get('site_name', 'Drupal') . ' - ' . $title;
          // Only display the description if we have a single term, to avoid clutter and confusion.
          if (count($tids) == 1) {
            $term = taxonomy_term_load($tids[0]);
            // HTML will be removed from feed description, so no need to filter here.
            $channel['description'] = $term->description;
          }

          $result = taxonomy_select_nodes($tids, $terms['operator'], $depth, FALSE, $order);
          $items = array();
          while ($row = db_fetch_object($result)) {
            $items[] = $row->nid;
          }

          node_feed($items, $channel);
          break;
        default:
          drupal_not_found();
      }
    }
    else {
      drupal_not_found();
    }
  }
}
