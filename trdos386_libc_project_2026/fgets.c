// fgets: Satýr bazlý ama tamponsuz doðrudan okuma
char* fgets(char *s, int size, FILE *fp) {
    int i = 0;
    while (i < size - 1) {
        int c = fgetc(fp);
        if (c == -1) break;
        s[i++] = (char)c;
        if (c == '\n' || c == '\r') break; // Enter'a basýldýysa dur
    }
    s[i] = '\0';
    return (i == 0) ? NULL : s;
}