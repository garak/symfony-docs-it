.. index::
   single: Form; Tipo di campo personalizzato

Come creare un tipo di campo personalizzato di un form
======================================================

Symfony è dotato di una serie di tipi di campi per la costruzione dei form.
Tuttavia ci sono situazioni in cui è necessario realizzare un campo personalizzato
per uno scopo specifico. Questa ricetta ipotizza che si abbia necessità 
di un capo personalizzato che contenga il genere di una persona, 
un nuovo campo basato su un campo di tipo scelta. Questa sezione spiega come il campo è definito, come si può personalizzare il layout e, infine, 
come è possibile registrarlo per utilizzarlo nell'applicazione.

Definizione del tipo di campo
-----------------------------

Per creare il tipo di campo personalizzato, è necessario creare per prima la classe
che rappresenta il campo. Nell'esempio proposto la classe che realizza il tipo di campo
sarà chiamata `GenderType` e il file sarà salvato nella cartella default contenente
i capi del form, che è ``<BundleName>\Form\Type``. Assicurati che il campo estenda
:class:`Symfony\\Component\\Form\\AbstractType`::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;

    class GenderType extends AbstractType
    {
        public function getDefaultOptions(array $options)
        {
            return array(
                'choices' => array(
                    'm' => 'Male',
                    'f' => 'Female',
                )
            );
        }

        public function getParent(array $options)
        {
            return 'choice';
        }

        public function getName()
        {
            return 'gender';
        }
    }

.. tip::

    La cartella di memorizzazione di questo file non è importante: la cartella ``Form\Type``
    è solo una convenzione.

Qui, il valore di ritorno del metodo ``getParent`` indica che che si sta
estendendo il tipo di campo ``choice``. Questo significa che di default, sono ereditate
tutte le logiche e la resa di queto tipo di campo. Per vedere alcune logiche,
controlla la classe `ChoiceType`_. Ci sono tre metodi che sono particolarmente
importanti:

* ``buildForm()`` - Ogni tipo di campo possiede un metodo ``buildForm``, che permette di
  configurare e creare ogni campo/campi. Notare che questo è lo stesso metodo 
  che è utilizzato per la preparazione  del *proprio* form, e qui funziona allo stesso.

* ``buildView()`` - Questo metodo è utilizzato per impostare le altre variabili che sono necessarie
  per la resa del campo nel template. Per esempio, nel tipo di campo `ChoiceType`_,
  la variabile ``multiple`` è impostata e utilizzata nel template  per impostare (o non 
  impostare) l'attributo ``multiple`` nel campo ``select``. Si faccia riferimento a `Creazione del template per il campo`_
  per maggiori dettagli.

* ``getDefaultOptions()`` - Questo metodo definisce le opzioni per il tipo di form
  che possono essere utilizzate in ``buildForm()`` e ``buildView()``. Ci sono molte 
  opzioni comuni a tutti i campi (vedere `FieldType`_), ma è possibile crearne altre,
  quante sono necessarie.

.. tip::

    Se si sta creando un campo che consiste di molti campi, assicurarsi  
    di impostare come "padre" un tipo come ``form`` o qualcos'altro che estenda ``form``.
    Nello stesso modo, se occorre modificare la "vista" di ogni sottotipo 
    che estende il proprio tipo, utilizzare il metodo ``buildViewBottomUp()``.

Il metodo ``getName()`` restituisce un identificativo che dovrebbe essere unico
all'interno dell'applicazione. Questo è usato in vari posti, ad esempio nel momento in cui 
il tipo di form è reso.

L'obiettivo del nostro tipo di campo era di estendere il tipo choice per permettere la selezione
del genere. Ciò si ottiene impostando in maniera fissa le ``choices`` con la lista
dei generi.

Creazione del template per il campo
-----------------------------------

Ogni campo è reso da un template, che è determinato in
parte dal valore del metodo ``getName()``. Per maggiori informazioni, vedere
:ref:`cookbook-form-customization-form-themes`.

In questo caso, dato che il campo padre è ``choice``, non è *necessario* fare
altre attività e il tipo di campo creato sarà automaticamente reso come tipo ``choice``. 
Ma per avere un esempio più incisivo, supponiamo che il tipo di campo creato
sia "expanded" (ad es. radio button o checkbox, al posto di un campo select),
vogliamo sempre la resa del campo in un elemento ``ul``. Nel template del proprio form
(vedere il link sopra per maggiori dettagli), creare un blocco ``gender_widget`` per gestire questo caso:

.. code-block:: html+jinja

    {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}

    {% block gender_widget %}
    {% spaceless %}
        {% if expanded %}
            <ul {{ block('widget_container_attributes') }}>
            {% for child in form %}
                <li>
                    {{ form_widget(child) }}
                    {{ form_label(child) }}
                </li>
            {% endfor %}
            </ul>
        {% else %}
            {# far rendere il tag select al widget choice #}
            {{ block('choice_widget') }}
        {% endif %}
    {% endspaceless %}
    {% endblock %}

.. note::

    Assicurarsu che il prefisso del widget utilizzato sia corretto. In questo esempio il nome dovrebbe
    essere ``gender_widget``, in base al valore restituito da ``getName``.
    Inoltre, il file principale di configurazione dovrebbe puntare al template personalizzato
    del form, in modo che sia utilizzato per la resa di tutti i form.

    .. code-block:: yaml

        # app/config/config.yml

        twig:
            form:
                resources:
                    - 'AcmeDemoBundle:Form:fields.html.twig'

Utilizzare il tipo di campo
---------------------------

Ora si può utilizzare il tipo di campo immediatamente, creando semplicemente una
nuova istanza del tipo in un form::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilder;
    
    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('gender_code', new GenderType(), array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

Questo funziona perché il ``GenderType()`` è veramente semplice. Cosa succede se
i valori del genere sono stati inseriti nella configurazione o nel database? La prossima
sezione spiega come un tipo di campo più complesso può risolvere questa situazione.

Creazione di un tipo di campo come servizio
-------------------------------------------

Finora, questa spiegazione ha assunto che si ha un tipo di campo molto semplice.
Ma se fosse necessario accedere alla configurazione o al database o a qualche altro
servizio, è necessario registrare il tipo di campo come servizio. Per
esempio, si supponga che i valori del genere siano memorizzati nella configurazione:

.. configuration-block::

    .. code-block:: yaml
    
        # app/config/config.yml
        parameters:
            genders:
                m: Male
                f: Female

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="genders" type="collection">
                <parameter key="m">Male</parameter>
                <parameter key="f">Female</parameter>
            </parameter>
        </parameters>

Per utilizzare i parametri, è necessario definire il tipo di campo come un servizio, iniettando
i valori dei parametri di ``genders`` come primo parametro del metodo
``__construct``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/services.yml
        services:
            form.type.gender:
                class: Acme\DemoBundle\Form\Type\GenderType
                arguments:
                    - "%genders%"
                tags:
                    - { name: form.type, alias: gender }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/services.xml -->
        <service id="form.type.gender" class="Acme\DemoBundle\Form\Type\GenderType">
            <argument>%genders%</argument>
            <tag name="form.type" alias="gender" />
        </service>

.. tip::

    Assicurarsi che il file dei servizi sia importato. Leggere :ref:`service-container-imports-directive`
    per dettagli.

Assicurarsi che l'attributo ``alias`` di tags corrisponda al valore restituito
dal metodo ``getName`` definito precedentemente. Si vedrà l'importanza
di questo nel momento in cui si utilizzerà il tipo di campo. Ma prima, si aggiunga al metodo ``__construct``
di ``GenderType`` un parametro, che riceverà la configurazione di gender::

    # src/Acme/DemoBundle/Form/Type/GenderType.php
    namespace Acme\DemoBundle\Form\Type;
    // ...

    class GenderType extends AbstractType
    {
        private $genderChoices;
        
        public function __construct(array $genderChoices)
        {
            $this->genderChoices = $genderChoices;
        }
    
        public function getDefaultOptions(array $options)
        {
            return array(
                'choices' => $this->genderChoices,
            );
        }
        
        // ...
    }

Benissimo! Il tipo ``GenderType`` è ora caricato con i parametri di configurazione ed è
registrato come servizio. In quanto nella configurazione del servizio si utilizza nel ``form.type`` l'alias,
utilizzare il campo risulta molto semplice::

    // src/Acme/DemoBundle/Form/Type/AuthorType.php
    namespace Acme\DemoBundle\Form\Type;
    // ...

    class AuthorType extends AbstractType
    {
        public function buildForm(FormBuilder $builder, array $options)
        {
            $builder->add('gender_code', 'gender', array(
                'empty_value' => 'Choose a gender',
            ));
        }
    }

Notare che al posto di creare l'istanza di una nuova istanza, ora è possibile riferirsi al tipo di campo
tramite l'alias utilizzato nella configurazione del servizio, ``gender``.

.. _`ChoiceType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/ChoiceType.php
.. _`FieldType`: https://github.com/symfony/symfony/blob/master/src/Symfony/Component/Form/Extension/Core/Type/FieldType.php
