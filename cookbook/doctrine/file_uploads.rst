.. index::
   single: Doctrine; Caricamento di file


Come gestire il caricamento di file con Doctrine
================================================

La gestione del caricamento dei file tramite le entità di Doctrine non è diversa
da qualsiasi altro tipo di caricamento. In altre parole, si è liberi di spostare 
il file nel controllore dopo aver gestito l'invio tramite un form. Per alcuni esempi
in merito fare riferimento alla pagina :doc:`dedicata al tipo file</reference/forms/types/file>`.

Volendo, è anche possibile integrare il caricamento del file nel ciclo di vita di
un'entità (creazione, modifica e cancellazione). In questo caso, nel momento
in cui l'entità viene creata, modificata o cancellata da Doctrine, il caricamento
del file o il processo di rimozione verranno azionati automaticamente (senza dover
fare nulla nel controllore);

Per far funzionare tutto questo è necessario conoscere alcuni dettagli, che verranno
analizzati in questa sezione del ricettario.

Preparazione
------------

Innanzitutto, creare una semplice classe entità di Doctrine, su cui lavorare::

    // src/Acme/DemoBundle/Entity/Document.php
    namespace Acme\DemoBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;

    /**
     * @ORM\Entity
     */
    class Document
    {
        /**
         * @ORM\Id
         * @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        public $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank
         */
        public $name;

        /**
         * @ORM\Column(type="string", length=255, nullable=true)
         */
        public $path;

        public function getAbsolutePath()
        {
            return null === $this->path
                ? null
                : $this->getUploadRootDir().'/'.$this->path;
        }

        public function getWebPath()
        {
            return null === $this->path
                ? null
                : $this->getUploadDir().'/'.$this->path;
        }

        protected function getUploadRootDir()
        {
            // il percorso assoluto della cartella dove i
            // documenti caricati verranno salvati
            return __DIR__.'/../../../../web/'.$this->getUploadDir();
        }

        protected function getUploadDir()
        {
            // togliamo __DIR_ in modo da visualizzare
            // correttamente nella vista il file caricato
            return 'uploads/documents';
        }
    }

L'entità ``Document`` ha un nome che viene associato al file. La proprietà ``path``
contiene il percorso relativo al file e viene memorizzata nella base dati. Il metodo
``getAbsolutePath()`` è un metodo di supporto che restituisce il percorso assoluto
al file, mentre ``getWebPath()`` è un altro metodo di supporto che restituisce
il percorso web, che può essere utilizzato nei template per collegare il file
caricato.

.. tip::

    Se non è già stato fatto, si consiglia la lettura della documentazione relativa
    al tipo :doc:`file</reference/forms/types/file>`, per comprendere meglio come
    funziona il caricamento di base.

.. note::

    Se si stanno utilizzando le annotazioni per specificare le regole di validazione
    (come nell'esempio proposto), assicurarsi di abilitare la validazione tramite
    annotazioni (confrontare :ref:`configurazione della validazione<book-validation-configuration>`).

Per gestire il file attualmente caricato tramite il form, utilizzare un campo
``file`` "virtuale". Per esempio, se si sta realizzando il form direttamente
nel controller, potrebbe essere come il seguente::

    public function uploadAction()
    {
        // ...

        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm();

        // ...
    }

In seguito, creare la proprietà nella classe ``Document`` aggiungendo alcune 
regole di validazione::

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/DemoBundle/Resources/config/validation.yml
        Acme\DemoBundle\Entity\Document:
            properties:
                file:
                    - File:
                        maxSize: 6000000

    .. code-block:: php-annotations

        // src/Acme/DemoBundle/Entity/Document.php
        namespace Acme\DemoBundle\Entity;

        // ...
        use Symfony\Component\Validator\Constraints as Assert;

        class Document
        {
            /**
             * @Assert\File(maxSize="6000000")
             */
            public $file;

            // ...
        }

    .. code-block:: xml

        <!-- src/Acme/DemoBundle/Resources/config/validation.yml -->
        <class name="Acme\DemoBundle\Entity\Document">
            <property name="file">
                <constraint name="File">
                    <option name="maxSize">6000000</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/DemoBundle/Entity/Document.php
        namespace Acme\DemoBundle\Entity;

        // ...
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Document
        {
            // ...
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('file', new Assert\File(array(
                    'maxSize' => 6000000,
                )));
            }
        }

.. note::

    Grazie al fatto che si utilizza il vincolo ``File``, Symfony2 ipotizzerà
    automaticamente che il campo del form sia un file upload. È per questo motivo
    che non si rende necessario impostarlo esplicitamente al momento di creazione del form precedente (``->add('file')``).

Il controllore seguente mostra come gestire l'intero processo::

    // ...
    use Acme\DemoBundle\Entity\Document;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    // ...

    /**
     * @Template()
     */
    public function uploadAction()
    {
        $document = new Document();
        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        if ($this->getRequest()->isMethod('POST')) {
            $form->bind($this->getRequest());
            if ($form->isValid()) {
                $em = $this->getDoctrine()->getManager();

                $em->persist($document);
                $em->flush();

                return $this->redirect($this->generateUrl(...));
            }
        }

        return array('form' => $form->createView());
    }

.. note::

    Realizzando il template non dimenticarsi di impostare l'attributo ``enctype``:

    .. configuration-block::

        .. code-block:: html+jinja

            <h1>Upload File</h1>

            <form action="#" method="post" {{ form_enctype(form) }}>
                {{ form_widget(form) }}

                <input type="submit" value="Upload Document" />
            </form>

        .. code-block:: html+php

            <h1>Upload File</h1>

            <form action="#" method="post" <?php echo $view['form']->enctype($form) ?>>
                <?php echo $view['form']->widget($form) ?>

                <input type="submit" value="Upload Document" />
            </form>

Il controllore precedente memorizzerà automaticamente l'entità ``Document`` con
il nome inviato, ma non farà nulla relativamente al file e la proprietà ``path``
sarà vuota.

Un modo semplice per gestire il caricamento del file è quello di spostarlo appena
prima che l'entità venga memorizzata, impostando la proprietà ``path`` in modo
corretto. Iniziare invocando un nuovo metodo ``upload()``, che si creerà tra poco
per gestire il caricamento del file, nella classe ``Document``::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getManager();

        $document->upload();

        $em->persist($document);
        $em->flush();

        return $this->redirect(...);
    }

Il metodo ``upload()`` sfrutterà l'oggetto :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`,
che è quanto viene restituito dopo l'invio di un campo di tipo ``file``::

    public function upload()
    {
        // la proprietà file può essere vuota se il campo non è obbligatorio
        if (null === $this->file) {
            return;
        }
        
        // si utilizza il nome originale del file ma è consigliabile
        // un processo di sanitizzazione almeno per evitare problemi di sicurezza
        
        // move accetta come parametri la cartella di destinazione
        // e il nome del file di destinazione
        $this->file->move(
            $this->getUploadRootDir(),
            $this->file->getClientOriginalName()
        );

        // impostare la proprietà del percorso al nome del file dove è stato salvato il file
        $this->path = $this->file->getClientOriginalName();

        // impostare a null la proprietà file dato che non è più necessaria
        $this->file = null;
    }

Utilizzare i callback del ciclo di vita delle entità
----------------------------------------------------

Anche se l'implementazione funziona, essa presenta un grave difetto: cosa succede
se si verifica un problema mentre l'entità viene memorizzata? Il file potrebbe
già essere stato spostato nella sua posizione finale anche se la proprietà
``path`` dell'entità non fosse stata impostata correttamente.

Per evitare questo tipo di problemi, è necessario modificare l'implementazione in
modo tale da rendere atomiche le azioni della base dati e dello spostamento del file:
se si verificasse un problema durante la memorizzazione dell'entità, o se il file non
potesse essere spostato, allora non dovrebbe succedere *niente*.

Per fare questo, è necessario spostare il file nello stesso momento in cui Doctrine
memorizza l'entità nella base dati. Questo può essere fatto agganciandosi a un callback
del ciclo di vita dell'entità::

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
    }

Quindi, rifattorizzare la classe ``Document``, per sfruttare i vantaggi dei callback::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                // fare qualsiasi cosa si voglia per generare un nome univoco
                $filename = sha1(uniqid(mt_rand(), true));
                $this->path = $filename.'.'.$this->file->guessExtension();
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // se si verifica un errore mentre il file viene spostato viene 
            // lanciata automaticamente un'eccezione da move(). Questo eviterà
            // la memorizzazione dell'entità nella base dati in caso di errore
            $this->file->move($this->getUploadRootDir(), $this->path);

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }
    }

La classe ora ha tutto quello che serve: genera un nome di file univoco prima
della memorizzazione, sposta il file dopo la memorizzazione, rimuove il file se
l'entità viene eliminata.

Ora che lo spostamento del file è gestito atomicamente dall'entità, la chiamata
a ``$document->upload()`` andrebbe tolta dal controllore::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getManager();

        $em->persist($document);
        $em->flush();

        $this->redirect(...);
    }

.. note::

    I callback ``@ORM\PrePersist()`` e ``@ORM\PostPersist()`` scattano prima e
    dopo la memorizzazione di un'entità nella base dati. Parallelamente, i callback
    ``@ORM\PreUpdate()`` e ``@ORM\PostUpdate()`` sono invocati quanto l'entità
    viene modificata.

.. caution::

    I callback ``PreUpdate`` e ``PostUpdate`` scattano solamente se c'è una modifica
    a uno dei campi dell'entità memorizzata. Questo significa che, se si modifica
    solamente la proprietà ``$file``, questi eventi non verranno invocati, dato che
    la proprietà in questione non viene memorizzata direttamente tramite Doctrine.
    Una soluzione potrebbe essere quella di utilizzare un campo ``updated`` memorizzato
    tramite Doctrine, da modificare manualmente in caso di necessità per la sostituzione del file.

Usare ``id`` come nome del file
-------------------------------

Volendo usare l'``id`` come nome del file, l'implementazione è leggermente
diversa, dato che sarebbe necessario memorizzare l'estensione nella proprietà
``path``, invece che nell'attuale nome del file::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        // una proprietà usata temporaneamente durante la cancellazione
        private $filenameForRemove;

        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                $this->path = $this->file->guessExtension();
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // qui si deve lanciare un'eccezione se il file non può essere spostato
            // per fare in modo che l'entità non possa essere memorizzata nella base dati
            // cosa che viene fatta da move()
            $this->file->move(
                $this->getUploadRootDir(),
                $this->id.'.'.$this->file->guessExtension()
            );

            unset($this->file);
        }

        /**
         * @ORM\PreRemove()
         */
        public function storeFilenameForRemove()
        {
            $this->filenameForRemove = $this->getAbsolutePath();
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($this->filenameForRemove) {
                unlink($this->filenameForRemove);
            }
        }

        public function getAbsolutePath()
        {
            return null === $this->path
                ? null
                : $this->getUploadRootDir().'/'.$this->id.'.'.$this->path;
        }
    }

Si noterà che in questo caso occorre un po' più di lavoro per poter rimuovere
il file. Prima che sia rimosso, si deve memorizzare il percorso del file
(perché dipende dall'id). Quindi, una volta che l'oggetto è completamente rimosso
dalla base dati, si può cancellare il file in sicurezza (dentro ``PostRemove``).
