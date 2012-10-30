.. index::
   single: Console; Applicazione in un singolo comando

Come costruire un'applicazione in un singolo comando
====================================================

Quando si costruisce uno strumento a linea di comando, potrebbe non essere necessario fornire
molti comandi. In questo caso, dover passare ogni volta il nome del comando potrebbe
essere noioso. Per fortuna, è possibile rimuovere questo obbligo, estendendo l'applicazione::

    namespace Acme\Tool;

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Input\InputInterface;

    class MiaApplicazione extends Application
    {
        /**
         * Restituisce il nome del comando, in base all'input
         *
         * @param InputInterface $input L'interfaccia input
         *
         * @return string Il nome del comando
         */
        protected function getCommandName(InputInterface $input)
        {
            // Deve restituire il nome del comando
            return 'mio_comando';
        }

        /**
         * Restituice i comandi predefiniti, che sono sempre disponibili
         *
         * @return array Un array di comandi predefiniti
         */
        protected function getDefaultCommands()
        {
            // Mantenere i comandi del nucleo, per avere HelpCommand,
            // usato quando si passa l'opzione --help
            $defaultCommands = parent::getDefaultCommands()

            $defaultCommands[] = new MioComando();

            return $defaultCommands;
        }
    }

Richiamando lo script da console, sarà sempre usato il comando `MioComando`,
senza doverne passare il nome.
