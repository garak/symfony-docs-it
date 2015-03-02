.. index::
   single: Bundle; Ereditarietà

Usare l'ereditarietà per sovrascrivere parti di un bundle
=========================================================

Lavorando con bundle di terze parti, ci si troverà probabilmente in situazioni in cui
si vuole sovrascrivere un file in quel bundle con un file del proprio bundle.
Symfony fornisce un modo molto conveniente per sovrascrivere cose come
controllori, template, traduzioni e altri file nella cartella ``Resources/``
di un bundle.

Per esempio, si supponga di aver installato `FOSUserBundle`_, ma di voler sovrascrivere
il suo template base ``layout.html.twig``, così come uno dei suoi
controllori. Si supponga anche di avere il proprio AcmeUserBundle,
in cui si vogliono mettere i file sovrascritti. Si inizi registrando FOSUserBundle
come "genitore" del proprio bundle::

    // src/Acme/UserBundle/AcmeUserBundle.php
    namespace Acme\UserBundle;

    use Symfony\Component\HttpKernel\Bundle\Bundle;

    class AcmeUserBundle extends Bundle
    {
        public function getParent()
        {
            return 'FOSUserBundle';
        }
    }

Con questa semplice modifica, si possono ora sovrascrivere diverse parti di FOSUserBundle,
semplicemente creando un file con lo stesso nome.

.. note::

    A dispetto del nome del metodo, non c'è una relazione genitore/figlio tra i
    bundle, è solo un modo per estendere e sovrascrivere un bundle esistente.

Sovrascrivere i controllori
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si supponga di voler aggiungere alcune funzionalità a ``registerAction`` di un
``RegistrationController``, che sta dentro ``FOSUserBundle``. Per poterlo fare,
creare un proprio ``RegistrationController.php``, ridefinire i metodi originali del
bundle e cambiarne le funzionalità::

    // src/Acme/UserBundle/Controller/RegistrationController.php
    namespace Acme\UserBundle\Controller;

    use FOS\UserBundle\Controller\RegistrationController as BaseController;

    class RegistrationController extends BaseController
    {
        public function registerAction()
        {
            $response = parent::registerAction();

            // ... fare qualcosa
            return $response;
        }
    }

.. tip::

    A seconda di quanto si vuole cambiare il comportamento, si potrebbe voler
    richiamare ``parent::registerAction()`` oppure sostituirne completamente
    la logica con una propria.

.. note::

    Sovrascrivere i controllori in questo modo funziona solo se il bundle fa
    riferimento al controllore tramite la sintassi standard ``FOSUserBundle:Registration:register``
    in rotte e template. Questa è la best practice.

Sovrascrivere risorse: template, rotte, traduzioni, validazione, ecc.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Anche molte risorse possono essere sovrascritte, semplicemente creando un file con lo
stesso percorso del bundle genitore.

Per esempio, è molto comune l'esigenza di sovrascrivere il template ``layout.html.twig`` di
``FOSUserBundle``, in modo che usi il layout di base dell'applicazione.
Poiché il file si trova in ``Resources/views/layout.html.twig`` di FOSUserBundle,
si può creare il proprio file nello stesso posto in AcmeUserBundle.
Symfony ignorerà completamente il file dentro FOSUserBundle e
userà il nuovo file al suo posto.

Lo stesso vale per i file delle rotte, della configurazione della validazione e di altre risorse.

.. note::

    La sovrascrittura di risorse funziona solo se ci si riferisce alle risorse col
    metodo ``@FosUserBundle/Resources/config/routing/security.xml``.
    Se ci si riferisce alle risorse senza usare la scorciatoia @NomeBundle, non
    possono essere sovrascritte in tal modo.

.. caution::

   I file delle traduzioni e delle validazioni non funzionano nel modo descritto sopra.
   Si legga ":ref:`override-translations`", se si vuole sapere come sovrascrivere le
   traduzioni e si veda ":ref:`override-validation`" per trucchi su come
   sovrascrivere la validazione.

.. _`FOSUserBundle`: https://github.com/friendsofsymfony/fosuserbundle
