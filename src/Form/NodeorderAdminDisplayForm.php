<?php
/**
 * @file
 * Contains \Drupal\nodeorder\Form\NodeorderAdminDisplayForm.
 */
namespace Drupal\nodeorder\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;
use Drupal\taxonomy\Entity\Term;


/**
 * Implements an example form.
 */
class NodeorderAdminDisplayForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'nodeorder_admin_display';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {

    $tid = \Drupal::request()->attributes->get('taxonomy_term');
    $node_ids = db_select('taxonomy_index', 'ti')
      ->fields('ti', ['nid', 'weight'])
      ->condition('ti.tid', $tid)
      ->orderBy('ti.weight')
      ->execute()
      ->fetchAllKeyed();
    $nodes = Node::loadMultiple(array_keys($node_ids));

    $term = Term::load($tid);

    $form['#title'] = $this->t('Order nodes for <em>%term_name</em>', array('%term_name' => $term->label()));
    $form['#term_id'] = $tid;

    $form['nodes'] = array(
      '#type' => 'table',
      '#header' => array(t('Title'), t('Weight')),
      '#empty' => t('No nodes exist in this category.'),
      '#tabledrag' => array(
        array(
          'action' => 'order',
          'relationship' => 'sibling',
          'group' => 'nodes-order-weight',
        ),
      ),
    );

    foreach ($nodes as $id => $entity) {
      $form['nodes'][$id]['#attributes']['class'][] = 'draggable';
      $form['nodes'][$id]['#weight'] = $entity->id();
      $form['nodes'][$id]['label'] = array(
        '#plain_text' => $entity->label(),
      );


      $form['nodes'][$id]['weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight for @title', array('@title' => $entity->label())),
        '#title_display' => 'invisible',
        '#default_value' => $entity->id(),
        '#attributes' => array('class' => array('nodes-order-weight')),
      );
    }
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save order'),
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    // if (strlen($form_state->getValue('phone_number')) < 3) {
    //   $this->setFormError('phone_number', $form_state, $this->t('The phone number is too short. Please enter a full phone number.'));
    // }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $tid = $form['#term_id'];
    foreach ($form_state->getValue(array('nodes')) as $nid => $node) {
      // Only take form elements that are blocks.
      if (is_array($node) && array_key_exists('weight', $node)) {
        db_update('taxonomy_index')->fields(['weight' => $node['weight']])
          ->condition('tid', $tid)
          ->condition('nid', $nid)
          ->execute();
      }
    }

    drupal_set_message(t('The node orders have been updated.'));
    \Drupal::cache()->deleteAll();
  }
}
