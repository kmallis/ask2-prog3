# ask2-prog3

# E-Shop με Διαδικτυακή Επικοινωνία (IPC) και Ανώνυμους Αγωγούς

## Περιγραφή Έργου

Το έργο αυτό είναι η υλοποίηση ενός **ηλεκτρονικού καταστήματος (e-shop)** που χρησιμοποιεί την τεχνική της **διαδιεργασιακής επικοινωνίας (IPC)** μέσω ανώνυμων αγωγών (**anonymous pipes**) και τη λειτουργία **fork()** για τη διαχείριση των πελατών και των παραγγελιών.


### Λειτουργία

Το e-shop περιλαμβάνει:

- **Κατάλογο προϊόντων**: Κάθε προϊόν έχει περιορισμένο αριθμό διαθέσιμων τεμαχίων.
- **Παραγγελίες**: Ο κάθε πελάτης μπορεί να υποβάλει πολλές παραγγελίες με χρονικό διάστημα 1 δευτερολέπτου μεταξύ τους.
- **Διαθεσιμότητα**: Εάν το προϊόν είναι διαθέσιμο, ο πελάτης ενημερώνεται για την επιτυχία της παραγγελίας του, διαφορετικά ενημερώνεται ότι το προϊόν δεν είναι διαθέσιμο.
- **Χρόνος Εξυπηρέτησης**: Κάθε παραγγελία εξυπηρετείται με καθυστέρηση 0,5 δευτερολέπτων για κάθε προϊόν.
- **Αναφορά**: Μετά την ολοκλήρωση των παραγγελιών, το σύστημα εξάγει μια αναφορά που περιλαμβάνει στατιστικά για τα προϊόντα και τις παραγγελίες.

### Αναφορά


1. Περιγραφή προϊόντος
2. Αριθμός αιτημάτων για αγορά
3. Αριθμός τεμαχίων που πουλήθηκαν
4. Λίστα πελατών που δεν εξυπηρετήθηκαν (λόγω έλλειψης διαθεσιμότητας)

Στο τέλος της αναφοράς, εμφανίζεται το συνολικό αποτέλεσμα του e-shop:
- Συνολικός αριθμός παραγγελιών
- Συνολικός αριθμός επιτυχημένων παραγγελιών
- Συνολικός αριθμός αποτυχημένων παραγγελιών
- Συνολικός τζίρος
