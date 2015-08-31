.. index::
   single: Form; Estensione di tipo form

Creare un'estensione di un tipo form
====================================

I :doc:`tipi di campo di form personalizzati<create_custom_field_type>` sono ottimi
quando occorrono tipi di campo con uno scopo specifico, come un selettore di genere o
un campo per la partita IVA.

A volte, però, non servono veramente dei nuovi tipi di campo, ma piuttosto l'aggiunta
di caratteristiche a tipi già esistenti. Qui entrano in gioco le estensioni
del tipo form.

Le estensioni del tipo form hanno due casi d'uso principali:

#. Si vuole aggiungere una **caratteristica generica a molti tipi** (come
   aggiungere un testo di aiuto a ogni tipo di campo);
#. Si vuole aggiungere una **caratteristica specifica a un singolo tipo** (come
   aggiungere una caratteristica "scarica" al tipo di campo "file").

In entrambi i casi, si potrebbe raggiungere lo scopo con una resa personalizzata
del form oppure con tipi di campo personalizzati. Tuttavia, l'uso delle estensioni
del tipo form sono più chiare (limitando la logica di business nei template)
e più flessibili (si possono aggiungere più estensioni a un singolo
tipo form).

Le estensioni del tipo form possono fare la maggior parte delle cose fatte dai tipi di
campo personalizzati, ma invece di essere tipi, **si agganciano ai tipi esistenti**.

Immaginiamo di gestire un'entità chiamata ``Media`` e che tale media sia associato a
un file. Il form ``Media`` usa un tipo file, ma alla modifica dell'entità si vuole
vedere l'immagine associata, accanto al campo
input.

Ovviamente, si potrebbe farlo personalizzando la resa del campo in un
template. Ma le estensioni del tipo form consentono di farlo in modo DRY.

Definire l'estensione del tipo form
-----------------------------------

Il primo passo consiste nel creare la classe dell'estensione. Chiamiamola
``ImageTypeExtension``. Per convenzione, le estensioni dei form di solito si
pongono nella cartella ``Form\Extension`` di un bundle.

Quando si crea un'estensione, si può implementare l'interfaccia
:class:`Symfony\\Component\\Form\\FormTypeExtensionInterface`
o estendere la classe :class:`Symfony\\Component\\Form\\AbstractTypeExtension`.
Nella maggior parte dei casi, è più facile estendere la classe astratta::

    // src/Acme/DemoBundle/Form/Extension/ImageTypeExtension.php
    namespace Acme\DemoBundle\Form\Extension;

    use Symfony\Component\Form\AbstractTypeExtension;

    class ImageTypeExtension extends AbstractTypeExtension
    {
        /**
         * Restituisce il nome del tipo da estendere.
         *
         * @return string Il nome del tipo da estendere
         */
        public function getExtendedType()
        {
            return 'file';
        }
    }

Il solo metodo che **si deve** implementare è ``getExtendedType``.
Tale metodo è usato per indicare il nome del tipo form che sarà esteso
dalla nuova estensione.

.. tip::

    Il valore restituito dal metodo ``getExtendedType`` corrisponde a quello
    restituito dal metodo ``getName`` nella classe tipo form che si vuole
    estendere.

Oltre al metodo ``getExtendedType``, probabilmente si vorrà ridefinire
uno dei metodi seguenti:

* ``buildForm()``

* ``buildView()``

* ``setDefaultOptions()``

* ``finishView()``

Per maggiori informazioni su cosa facciano questi metodi, fare riferimento alla
ricetta
:doc:`creare tipi di campo personalizzati</cookbook/form/create_custom_field_type>`.

Registrare l'estensione del tipo form come servizio
---------------------------------------------------

Il passo successivo e far sapere a Symfony della nuova estensione. Tutto ciò che
serve è dichiararla come servizio, usando il tag
``form.type_extension``:

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo_bundle.image_type_extension:
                class: Acme\DemoBundle\Form\Extension\ImageTypeExtension
                tags:
                    - { name: form.type_extension, alias: file }

    .. code-block:: xml

        <service id="acme_demo_bundle.image_type_extension"
            class="Acme\DemoBundle\Form\Extension\ImageTypeExtension"
        >
            <tag name="form.type_extension" alias="file" />
        </service>

    .. code-block:: php

        $container
            ->register(
                'acme_demo_bundle.image_type_extension',
                'Acme\DemoBundle\Form\Extension\ImageTypeExtension'
            )
            ->addTag('form.type_extension', array('alias' => 'file'));

La chiave ``alias`` del tag è il tipo di campo a cui l'estensione va applicata.
Nel nostro caso, poiché si vuole estendere il tipo di campo ``file``,
si usa ``file`` come alias.

Aggiungere la logica di business
--------------------------------

Lo scopo dell'estensione è mostrare un'immagine accanto al campo input file
(quando il modello sottostante contiene immagini). A tale scopo, ipotizziamo di
usare un approccio simile a quello descritto in
:doc:`come gestire caricamenti di file con Doctrine</cookbook/doctrine/file_uploads>`:
abbiamo un modello ``Media`` con una proprietà ``file`` (che corrisponde al campo
``file`` nel form) e una proprietà ``path`` (che corrisponde al percorso dell'immagine
nella base dati)::

    // src/Acme/DemoBundle/Entity/Media.php
    namespace Acme\DemoBundle\Entity;

    use Symfony\Component\Validator\Constraints as Assert;

    class Media
    {
        // ...

        /**
         * @var string Il percorso, tipicamente memorizzato in base dati
         */
        private $path;

        /**
         * @var \Symfony\Component\HttpFoundation\File\UploadedFile
         * @Assert\File(maxSize="2M")
         */
        public $file;

        // ...

        /**
         * Restituisce l'url dell'immagine
         *
         * @return null|string
         */
        public function getWebPath()
        {
            // ... dove $webPath è l'url completo dell'immagine, da usare nei template

            return $webPath;
        }
    }

La classe dell'estensione ha bisogno di due cose per poter estendere
il tipo form ``file``:

#. Ridefinire il metodo ``setDefaultOptions`` per poter aggiungere un'opzione
   ``image_path``;
#. Ridefinire i metodi ``buildForm`` e ``buildView`` per poter passare l'url dell'immagine
   alla vista.

La logica è la seguente: quando si aggiunge un campo di tipo ``file``,
si potrà specificare una nuova opzione: ``image_path``. Tale opzione dirà
al campo file come recuperare l'url dell'immagine, per poterla mostrare
nella vista::

    // src/Acme/DemoBundle/Form/Extension/ImageTypeExtension.php
    namespace Acme\DemoBundle\Form\Extension;

    use Symfony\Component\Form\AbstractTypeExtension;
    use Symfony\Component\Form\FormView;
    use Symfony\Component\Form\FormInterface;
    use Symfony\Component\Form\Util\PropertyPath;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class ImageTypeExtension extends AbstractTypeExtension
    {
        /**
         * Restituisce il nome del tipo da estendere
         *
         * @return string Il nome del tipo da estendere
         */
        public function getExtendedType()
        {
            return 'file';
        }

        /**
         * Aggiunge l'opzione image_path
         *
         * @param \Symfony\Component\OptionsResolver\OptionsResolverInterface $resolver
         */
        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setOptional(array('image_path'));
        }

        /**
         * Passa l'url dell'immagine alla vista
         *
         * @param \Symfony\Component\Form\FormView $view
         * @param \Symfony\Component\Form\FormInterface $form
         * @param array $options
         */
        public function buildView(FormView $view, FormInterface $form, array $options)
        {
            if (array_key_exists('image_path', $options)) {
                $parentData = $form->getParent()->getData();

                if (null !== $parentData) {
                    $propertyPath = new PropertyPath($options['image_path']);
                    $imageUrl = $propertyPath->getValue($parentData);
                } else {
                     $imageUrl = null;
                }

                // imposta una variabile "image_url", che sarà disponibile quando si rende questo campo
                $view->set('image_url', $imageUrl);
            }
        }

    }

Ridefinire il frammento di template del widget File
---------------------------------------------------

Ogni tipo di campo viene resto da un frammento di template. Questi frammenti di template
possono essere ridefiniti, per poter personalizzare la resa del form. Per maggiori
informazioni, fare riferimento alla ricetta :ref:`cookbook-form-customization-form-themes`.

Nella classe estensione abbiamo aggiunto una nuova variabile (``image_url``), ma
dobbiamo ancora specificare cosa fare con tale variabile nei template.
Nello specifico, occorre sovrascrivere il blocco ``file_widget``:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/DemoBundle/Resources/views/Form/fields.html.twig #}
        {% extends 'form_div_layout.html.twig' %}

        {% block file_widget %}
            {% spaceless %}

            {{ block('form_widget') }}
            {% if image_url is not null %}
                <img src="{{ asset(image_url) }}"/>
            {% endif %}

            {% endspaceless %}
        {% endblock %}

    .. code-block:: html+php

        <!-- src/Acme/DemoBundle/Resources/views/Form/file_widget.html.php -->
        <?php echo $view['form']->widget($form) ?>
        <?php if (null !== $image_url): ?>
            <img src="<?php echo $view['assets']->getUrl($image_url) ?>"/>
        <?php endif ?>

.. note::

    Occorrerà modificare il file di configurazione o specificare esplicitamente
    il tema del form, per consentire a Symfony di usare il blocco
    sovrascritto. Vedere :ref:`cookbook-form-customization-form-themes` per maggiori
    informazioni.

Usare l'estensione
------------------

D'ora in poi, quando si aggiunge un tipo di campo ``file`` a un form, si può
specificare un'opzione ``image_path``, che sarà usata per mostrare un'immagine
vicino al campo file. Per esempio::

    // src/Acme/DemoBundle/Form/Type/MediaType.php
    namespace Acme\DemoBundle\Form\Type;

    use Symfony\Component\Form\AbstractType;
    use Symfony\Component\Form\FormBuilderInterface;

    class MediaType extends AbstractType
    {
        public function buildForm(FormBuilderInterface $builder, array $options)
        {
            $builder
                ->add('name', 'text')
                ->add('file', 'file', array('image_path' => 'webPath'));
        }

        public function getName()
        {
            return 'media';
        }
    }

Mostrando il form, se il modello sottostante ha già un'immagine associata,
questa sarà mostrata accanto al campo file.
