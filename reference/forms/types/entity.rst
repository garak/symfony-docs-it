.. index::
   single: Form; Campi; choice

Tipo di campo entity
====================

Un campo speciale ``choice`` pensato per caricare opzioni da un'entità Doctrine.
Per esempio, se si ha un'entità ``Category``, si può usare questo campo
per mostrare un campo ``select`` di tutti, o di alcuni, oggetti ``Category``
presi dalla base dati.

+---------------+---------------------------------------------------------------------+
| Reso come     | possono essere vari tag (vedere :ref:`forms-reference-choice-tags`) |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `class`_                                                          |
|               | - `property`_                                                       |
|               | - `query_builder`_                                                  |
|               | - `em`_                                                             |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `required`_                                                       |
| ereditate     | - `label`_                                                          |
|               | - `multiple`_                                                       |
|               | - `expanded`_                                                       |
|               | - `preferred_choices`_                                              |
|               | - `empty_value`_                                                    |
|               | - `read_only`_                                                      |
|               | - `error_bubbling`_                                                 |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`choice</reference/forms/types/choice>`                        |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Bridge\\Doctrine\\Form\\Type\\EntityType`          |
+---------------+---------------------------------------------------------------------+

Utilizzo di base
----------------

Il tipo ``entity`` ha una sola opzione obbligatoria: l'entità da elencare all'interno
del campo choice::

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
    ));

In questo caso, tutti gli oggetti ``User`` saranno caricati dalla base dati e resi come
un tag ``select``, dei radio o una serie di checkbox (a seconda dei valori di
``multiple`` ed ``expanded``).

Usare una query personalizzata per le entità
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Se occorre specificare una query personalizzata da usare per recuperare le entità
(p.e.e si vogliono solo alcune entità o le si vuole ordinare), usare l'opzione
``query_builder``. Il modo più facile è fare come segue::

    use Doctrine\ORM\EntityRepository;
    // ...

    $builder->add('users', 'entity', array(
        'class' => 'AcmeHelloBundle:User',
        'query_builder' => function(EntityRepository $er) {
            return $er->createQueryBuilder('u')
                ->orderBy('u.username', 'ASC');
        },
    ));

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

Opzioni del campo
-----------------

class
~~~~~

**tipo**: ``stringa`` **required**

La classe dell'entità (p.e. ``AcmeStoreBundle:Category``). Può essere
un nome di classe pienamente qualificato (p.e. ``Acme\StoreBundle\Entity\Category``)
o il suo alias (come mostrato sopra).

property
~~~~~~~~

**tipo**: ``stringa``

La proprietà da usare per mostrare le entità come testi nell'elemento HTML.
Se lasciata vuota, gli oggetti saranno formattati come stringhe, quindi
occorre avere un metodo ``__toString()``.

query_builder
~~~~~~~~~~~~~

**tipo**: ``Doctrine\ORM\QueryBuilder`` oppure una closure

Se specificato, è usato per cercare un sotto-insieme di opzioni (e il loro
ordina) che dovrebbero essere usate per il campo. Il valore di questa opzione
può essere un oggetto ``QueryBuilder`` oppure una closure. Se su usa una closure,
dovrebbe accettare un singolo parametro, che è l'``EntityRepository``
dell'entità.

em
~~

**tipo**: ``stringa`` **predefinito**: l'entity manager predefinito

Se specificato, l'entity manager da usare per caricare le scelte, al posto
di quello predefinito.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
