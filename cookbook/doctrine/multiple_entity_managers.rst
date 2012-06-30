.. index::
   single: Doctrine; Gestori di entità multipli

Come lavorare con gestori di entità multipli
============================================

Si possono usare gestori di entità multipli in un'applicazione Symfony2.
Questo si rende necessario quando si usano diverse basi dati o addirittura venditori
con insiemi di entità completamente differenti. In altre parole, un gestore di entità
che si connette a una base dati gestirà alcune entità, mentre un altro gestore di entità
che si connette a un'altra base dati potrebbe gestire il resto.

.. note::

    L'uso di molti gestori di entità è facile, ma più avanzato e solitamente non
    richiesto. Ci si assicuri di avere effettivamente bisogno di gestori di entità
    multipli, prima di aggiungere un tale livello di complessità.

La configurazione seguente mostra come configurare due gestori di entità:

.. configuration-block::

    .. code-block:: yaml

        doctrine:
            orm:
                default_entity_manager:   default
                entity_managers:
                    default:
                        connection:       default
                        mappings:
                            AcmeDemoBundle: ~
                            AcmeStoreBundle: ~
                    customer:
                        connection:       customer
                        mappings:
                            AcmeCustomerBundle: ~

In questo caso, sono stati definiti due gestori di entità, chiamati ``default``
e ``customer``. Il gestore di entità ``default`` gestisce le entità in
``AcmeDemoBundle`` e ``AcmeStoreBundle``, mentre il gestore di entità ``customer``
gestisce le entità in ``AcmeCustomerBundle``.

Lavorando con gestori di entità multipli, occorre esplicitare quale gestore di entità
si vuole usare. Se si *omette* il nome del gestore di entità al momento della sua
richiesta, verrà restituito il gestore di entità predefinito (cioè ``default``)::

    class UserController extends Controller
    {
        public function indexAction()
        {
            // entrambi restiuiscono "default"
            $em = $this->get('doctrine')->getEntityManager();
            $em = $this->get('doctrine')->getEntityManager('default');
            
            $customerEm =  $this->get('doctrine')->getEntityManager('customer');
        }
    }

Si può ora usare Doctrine come prima, usando il gestore di entità ``default`` per
persistere e recuperare le entità da esso gestite e il gestore di entità
``customer`` per persistere e recuperare le sue entità.
