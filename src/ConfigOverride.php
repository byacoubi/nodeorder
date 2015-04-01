<?php

/**
 * @file
 * Contains \Drupal\nodeorder\ConfigOverride.
 */

namespace Drupal\nodeorder;

use Drupal\Core\Config\ConfigFactoryOverrideInterface;

/**
 * Override the term listing view to sort by node order weight.
 */
class ConfigOverride implements ConfigFactoryOverrideInterface {

  /**
   * Override config for the term listing view.
   *
   * {@inheritdoc}
   */
  public function loadOverrides($names) {
    $overrides = array();
    if (in_array('views.view.taxonomy_term', $names)) {
      $overrides['views.view.taxonomy_term'] = $this->getOverriddenView();
    }
    return $overrides;
  }

  /**
   * {@inheritdoc}
   */
  public function getCacheSuffix() {
    return 'NodeorderConfigOverrider';
  }

  /**
   * {@inheritdoc}
   */
  public function createConfigObject($name, $collection = StorageInterface::DEFAULT_COLLECTION) {
    return NULL;
  }

  /**
   * Provide actual overridden view config.
   */
  protected function getOverriddenView() {
    $view = array(
      'display' => array(
        'default' => array(
          'display_options' => array(
            'sorts' => array(
              // We need to redeclare other sorts here too, since the order matters!!.
              // @todo see https://www.drupal.org/node/2416129
              'sticky' => NULL,
              'weight' => array(
                'id' => 'weight',
                'table' => 'taxonomy_index',
                'field' => 'weight',
                'relationship' => 'none',
                'group_type' => 'group',
                'admin_label' => '',
                'order' => 'ASC',
                'exposed' => false,
                'expose' => array(
                  'label' => '',
                ),
                'plugin_id' => 'standard',
              ),
              'created' => NULL,
            ),
          ),
        ),
      ),
    );
    return $view;
  }

}
