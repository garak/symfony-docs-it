format
~~~~~~

**tipo**: ``intero`` o ``stringa`` **predefinito**: ``IntlDateFormatter::MEDIUM``

Opzione passata alla classe ``IntlDateFormatter``, usata per trasformare il dato inserito
dall'utente nel formato appropriato. Questè è molto importante quando l'opzione `widget`_
è ``single_text``, e definisce il modo in cui il dato viene trasformato. Per impostazione
predefinita, il formato è determinato in base al locale dell'utente; si può sovrascriverlo
passando il formato come stringa.
