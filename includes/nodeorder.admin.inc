<?php

/**
 * @file
 * Admin page callbacks for the nodeorder module.
 */

/**
 * Form for Admin Settings
 */
function nodeorder_admin($form, &$form_state) {
  $form['nodeorder_show_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display ordering links'),
    '#description' => t('Choose whether to show ordering links. Links can be shown for all categories associated to a node or for the currently active category. It is also possible to not show the ordering links at all.'),
  );
  $form['nodeorder_show_links']['nodeorder_show_links_on_node'] = array(
    '#type' => 'radios',
    '#title' => t('Choose how to display ordering links'),
    '#default_value' => \Drupal::config('nodeorder.settings')->get('nodeorder_show_links_on_node'),
    '#description' => 'When displaying links based on the context, they will only be shown on taxonomy and nodeorder pages.',
    '#options' => array(t('Don\'t display ordering links'), t('Display ordering links for all categories'), t('Display ordering links based on the active category')),
  );

  $form['nodeorder_link_to_ordering_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link to the ordering page'),
    '#description' => t('If enabled, a tab will appear on all <em>nodeorder/term/%</em> and <em>taxonomy/term/%</em> pages that quickly allows administrators to get to the node ordering administration page for the term.'),
    '#default_value' => \Drupal::config('nodeorder.settings')->get('nodeorder_link_to_ordering_page'),
  );

  $form['nodeorder_link_to_ordering_page_taxonomy_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display link to the ordering page on taxonomy administration page'),
    '#description' => t('If enabled, a tab will appear on <em>admin/content/taxonomy/%</em> pages that quickly allows administrators to get to the node ordering administration page for the term.'),
    '#default_value' => \Drupal::config('nodeorder.settings')->get('nodeorder_link_to_ordering_page_taxonomy_admin'),
  );

  $form['nodeorder_override_taxonomy_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override the default taxonomy page with one from nodeorder'),
    '#description' => t('Disabling this will allow the panels module to override taxonomy pages instead. See <a href="https://www.drupal.org/node/1713048">this issue</a> for more information. You will have to clear caches for the change to take effect.'),
    '#default_value' => \Drupal::config('nodeorder.settings')->get('nodeorder_override_taxonomy_page'),
  );

  return system_settings_form($form);
}

/**
 * Returns HTML for the vocabulary overview form as a sortable list of vocabularies.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see taxonomy_overview_vocabularies()
 * @ingroup themeable
 */
function theme_nodeorder_admin_display_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('nodeorder', 'order', 'sibling', 'node-weight');

  $errors = form_get_errors() != FALSE ? form_get_errors() : array();
  $rows = array();
  foreach (\Drupal\Core\Render\Element::children($form) as $key) {
    if (isset($form[$key]['#node'])) {
      $node = &$form[$key];

      $row = array();
      $row[] = drupal_render($node['title']);
      $node['weight']['#attributes']['class'] = array('node-weight');
      $row[] = drupal_render($node['weight']);
      $row = array('data' => $row);
      $rows[$key] = $row;
    }
  }

  // Add necessary classes to rows.
  $row_position = 0;
  foreach ($rows as $key => $row) {
    $rows[$key]['class'] = array();
    $rows[$key]['class'][] = 'draggable';

    // Add an error class if this row contains a form error.
    foreach ($errors as $error_key => $error) {
      if (strpos($error_key, $key) === 0) {
        $rows[$key]['class'][] = 'error';
      }
    }
    $row_position++;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('There are no nodes for this term.'), 'colspan' => '3'));
  }

  $header = array(t('Title'), t('Weight'), );
  $output = _theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'nodeorder')));
  $output .= drupal_render_children($form);

  return $output;
}
