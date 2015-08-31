.. index::
   single: Form; Dati vuoti

Configurare dati vuoti per una classe Form
===========================================

L'opzione ``empty_data`` consente di specificare un insieme di dati vuoti per una
classe form. I dati vuoti saranno usati all'invio del form, se non è stato richiamato
``setData()`` sul form né sono stati passati dati alla creazione
del form. Per esempio::

    public function indexAction()
    {
        $blog = ...;

        // $blog viene passato come dato, quindi l'opzione
        // empty_data non è necessaria
        $form = $this->createForm(new BlogType(), $blog);

        // nessun dato passato, quindi viene usata
        // empty_data per ottenere "dati iniziali"
        $form = $this->createForm(new BlogType());
    }

Il valore predefinito di ``empty_data`` è ``null``. Se invece è stata specificata
un'opzione ``data_class`` nella classe form, il valore predefinito sarà una nuova istanza
di tale classe. Questa istanza sarà creata richiamando il costruttore
senza parametri.

Se si vuole ridefinire questo comportamento, ci sono due possibili modi.

Opzione 1: istanziare una nuova classe
--------------------------------------

Una ragione per voler usare questa opzione è se si vuole usare un costruttore con
parametri. Si ricorda che l'opzione predefinita di ``data_class`` richiama
il costruttore senza parametri::

    // src/AppBundle/Form/Type/BlogType.php

    // ...
    use Symfony\Component\Form\AbstractType;
    use AppBundle\Entity\Blog;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class BlogType extends AbstractType
    {
        private $someDependency;

        public function __construct($someDependency)
        {
            $this->someDependency = $someDependency;
        }
        // ...

        public function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            $resolver->setDefaults(array(
                'empty_data' => new Blog($this->someDependency),
            ));
        }
    }

Si può istanziare la classe nel modo preferito. In questo esempio, viene passata
una dipendenza a ``BlogType`` durante l'istanza, quindi la si usa
per istanziare l'oggetto ``Blog``. Il punto è che si può impostare ``empty_data``
al nuovo oggetto che si vuole usare.

Opzione 2: fornire una closure
------------------------------

L'uso di una closure è il metodo preferito, perché creerà l'oggetto solo
se necessario.

La closure deve accettare un'istanza di ``FormInterface`` come primo parametro::

    use Symfony\Component\OptionsResolver\OptionsResolverInterface;
    use Symfony\Component\Form\FormInterface;
    // ...

    public function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setDefaults(array(
            'empty_data' => function (FormInterface $form) {
                return new Blog($form->get('title')->getData());
            },
        ));
    }
