.. index::
   single: Bundle; Ereditarietà

Come sovrascrivere parti di un bundle
=====================================

Questa ricetta è un riferimento veloce su come sovrascrivere le varie parti di un
bundle di terze parti.

Template
--------

Per informazioni su come sovrascrivere i template, vedere

* :ref:`overriding-bundle-templates`.
* :doc:`/cookbook/bundles/inheritance`

Rotte
-----

Le rotte non sono mai importate automaticamente in Symfony2. Se si vogliono includere rotte
da un bundle, occorre importarle manualmente da qualche parte nella
propria applicazione (p.e. ``app/config/routing.yml``).

Il modo migliore per "sovrascrivere" le rotte di un bundle è quello di non importarle
affatto. Invece di importare le rotte di un bundle di terze parti, meglio copiare
il file delle rotte nella propria applicazione, modificarlo e importare quello.

Controllori
-----------

Ipotizzando che un bundle di terze parti usi controllori che non siano servizi (che
è quasi sempre il caso), si possono facilmente sovrascrivere tramite l'ereditarietà
dei bundle. Per maggiori informazioni, vedere :doc:`/cookbook/bundles/inheritance`.
Se il controllore è un servizio, vedere la sezione successiva su come sovrascriverlo.

Servizi & configurazione
------------------------

Ci sono due opzioni per poter estendere o sovrascrivere un servizio. La prima è
impostare il parametro che contiene il nome della classe del servizio, cambiandolo con la propria
classe in ``app/config/config.yml``. Ovviamente, questo è possibile solo se il nome della classe è
definito come parametro nella configurazione del servizio del bundle che contiene il
servizio. Per esempio, per sovrascrivere la classe usata dal servizio ``translator`` di Symfony,
si può sovrascrivere il parametro ``translator.class``. Potrebbe essere necessaria un po' di
ricerca per sapere quale parametro sovrascrivere. Per il traduttore, il parametro è
definito e usato nel file ``Resources/config/translation.xml`` di
FrameworkBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            translator.class:      Acme\HelloBundle\Translation\Translator

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="translator.class">Acme\HelloBundle\Translation\Translator</parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        $container->setParameter('translator.class', 'Acme\HelloBundle\Translation\Translator');

Come seconda opzione, se la classe non è disponibile come parametro, ci si potrebbe assicurare
che la classe sia sempre sovrascritta quando il proprio bundle viene usato oppure usare
un passo di compilatore, per modificare qualcosa che non sia solamente il nome della classe::

    // src/Acme/DemoBundle/DependencyInjection/Compiler/OverrideServiceCompilerPass.php
    namespace Acme\DemoBundle\DependencyInjection\Compiler;

    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class OverrideServiceCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $container)
        {
            $definition = $container->getDefinition('id-del-servizio-originale');
            $definition->setClass('Acme\DemoBundle\UnServizio');
        }
    }

In questo esempio, recuperiamo la definizione del servizio originale e impostiamo
il nome della sua classe nella nostra classe.

Si veda :doc:`/cookbook/service_container/compiler_passes` per informazioni su come usare
i passi di compilatore. Se non si vuole solo sovrascrivere la classe, per esempio si vuole
aggiungere la chiamata a un metodo, l'unica opzione è il passo di compilatore.

Entità & mappature
------------------

Per come funziona Doctrine, non è possibile sovrascrivere la mappatura di entità
di un bundle. Tuttavia, se il bundle fornisce una superclasse mappata (come
l'entità ``User`` in FOSUserBundle), si possono sovrascrivere attributi e
associazioni. Si può approfondire questa caratteristica e i suoi limiti nella
`documentazione di Doctrine`_.

Form
----

Per poter sovrascrivere un tipo di form, questo deve essere registrato come servizio (con
il tag "form.type"). Lo si può quindi sovrascrivere come qualsiasi altro servizio, come
descritto in `servizi & configurazione`_. Ovviamente, funziona solo se il tipo
è referenziato tramite alias, piuttosto che istanziato,
p.e.::

    $builder->add('nome', 'tipo_personalizzato');

e non::

    $builder->add('nome', new TipoPersonalizzato());

Meta-dati di validazione
------------------------

In corso...

.. _override-translations:

Traduzioni
----------

Le traduzioni sono riguardano i bundle, ma i domini. Questo vuol dire che
si possono sovrascrivere traduzioni per qualsiasi file di traduzione, purché si trovi
nel :ref:`dominio corretto <using-translation-domains>`.

.. caution::

    L'ultimo file di traduzione vince. Questo vuol dire che occorre assicurarsi
    che il bundle contenente le *proprie* traduzioni sia caricato dopo ogni
    bundle con traduzioni da sovrascrivere. Lo si fa in ``AppKernel``.

    Il file che vince sempre è quello posto in
    ``app/Resources/translations``, poiché è sempre caricato per ultimo.

.. _`documentazione di Doctrine`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/inheritance-mapping.html#overrides
