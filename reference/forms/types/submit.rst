.. index::
   single: Form; Campi; submit

submit Field Type
=================

.. versionadded:: 2.3
    Il tipo ``submit`` è stato aggiunto in Symfony 2.3

A submit button.

+----------------------+----------------------------------------------------------------------+
| Reso come            | tag ``input`` ``submit``                                             |
+----------------------+----------------------------------------------------------------------+
| Opzioni              | - `attr`_                                                            |
| ereditate            | - `disabled`_                                                        |
|                      | - `label`_                                                           |
|                      | - `label_attr`_                                                      |
|                      | - `translation_domain`_                                              |
|                      | - `validation_groups`_                                               |
+----------------------+----------------------------------------------------------------------+
| Tipo genitore        | :doc:`button</reference/forms/types/button>`                         |
+----------------------+----------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SubmitType` |
+----------------------+----------------------------------------------------------------------+

Il bottone Submit button ha un metodo aggiuntivo
:method:`Symfony\\Component\\Form\\ClickableInterface::isClicked`, che consente
di verificare se questo bottone sia stato usato per inviare il form. Questo è utile
specialmente quando :ref:`un form ha più bottoni submit <book-form-submitting-multiple-buttons>`::

    if ($form->get('save')->isClicked()) {
        // ...
    }

Opzioni ereditate
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc

validation_groups
~~~~~~~~~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``null``

Quando un form contiene più bottoni submit, si può modificare il gruppo di
validazione, in base al bottone usato per inviare il form. Si immagini un form di
registrazione in più passi, con bottoni per andare al passo precedente o al successivo::

    $form = $this->createFormBuilder($user)
        ->add('precedente', 'submit', array(
            'validation_groups' => false,
        ))
        ->add('successivo', 'submit', array(
            'validation_groups' => array('Registration'),
        ))
        ->getForm();

Il valore speciale ``false`` assicura che non venga eseguita alcuna validazione quando
si clicca il bottone per andare indietro. Quando si clicca l'altro bottone, vengono validati
tutti i vincoli del gruppo "Registration".

.. seealso::

    Si può approfondire l'argomento nel :ref:`capitolo Form <book-form-validation-groups>`
    del libro.

Variabili di form
-----------------

========= ============ ==============================================================
Variabile Tipo         Uso
========= ============ ==============================================================
clicked   ``Booleano`` Se il bottone sia stato cliccato o meno.
========= ============ ==============================================================
