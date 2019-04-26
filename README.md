# SoalShift_modul4_E15

### Nomor 1

Buat fungsi encrypt dan decrypt, nantinya setiap fungsi akan encrypt path yang ada dan jika dibutuhkan menampilkan list file dan lain sebagainya maka fungsi decrypt akan dipanggil.

Kedua fungsi akan berjalan dengan mencari karakter yang ada di chiper lalu menambahkannya dengan key yang ada.

### Nomor 3

Perubahan ada pada fungsi readdir ketika menemukan persyaratan yang ditentukan maka masukkan ke file yang diminta.

### Nomor 4

Perubahan fungsi mkdir, utimens, create, chmod.

### Nomor 5

Fungsi yang dibutuhkan yaitu write untuk mengedit file, truncate untuk mengubah ukuran file dan juga unlink untuk menghapus.

Untuk edit filenya, maka yang di ubah di write setelah pwrite. Setelah pemanggilan fungsi akan dilakukan copi isi dari file yang diubah. Hal ini membutuhkan variabel time, struct tm(yang nantinya ada informasi seperti tanggal, waktu yang digenerate dari variabel time). Lalu untuk copy membuat fungsi tersendiri yaitu sebagai berikut:

```
void copy(char *source, char *dest)
{
	FILE *fptr1, *fptr2; 
  
    // Open one file for reading 
    fptr1 = fopen(source, "r"); 
    if (fptr1 == NULL) 
    { 
        exit(0); 
    } 
  
    // Open another file for writing 
    fptr2 = fopen(dest, "w"); 
    if (fptr2 == NULL) 
    { 
        exit(0); 
    } 
  
    // Read contents from file 
    char c = fgetc(fptr1); 
    while (c != EOF) 
    { 
        fputc(c, fptr2); 
        c = fgetc(fptr1); 
    } 
  
    fclose(fptr1); 
    fclose(fptr2); 
}
```

Untuk zip, ada pengecualian jika ekstensi adalah .swp maka abaikan. Jika ya, maka ambil nama file yang dihapus. Lakukan lakukan zip dengan nama file*.
