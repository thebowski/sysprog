
#include "diskdriver.h"

uint32_t part4_lba;


int32_t disk_demo( void ){
    disk_init();

    uint8_t count2[512*2];
    for (int i=0; i<512*2; i++) count2[i]=i/2;
    uint8_t count4[512*4];
    for (int i=0; i<512*4; i++) count4[i]=i/8;

    uint8_t data[512];

    uint8_t long_array[65536];

    uint8_t buff[256];

    int time;

    //time = gettime();
    //writefile("short2", count2, 16);
    //readfile("short2", buff, 16);
    //c_printf("Start time: %d\n", gettime());
    //c_printf("End time: %d\n", gettime());
    //print_data(buff, 0, 16);

    //time = gettime();
    //writefile("long", long_array, 65535);
    //readfile("long", long_array, 65535);
    //c_printf("Start time: %d\n", time);
    //c_printf("End time: %d\n", gettime());
    //
    //

    writefile("hello", count2, 512);
    read_sector(data, part4_lba);
    print_data(data, 0, 64);
    clearfiles();
    read_sector(data, part4_lba);
    print_data(data, 0, 64);

    readfile("short", buff, 10);
    read_sector(data, part4_lba);
    print_data(data, 0, 64);

    //print_data(data, 0, 16);
    //readfile("short",data,10);
    //print_data(data, 0, 10);
    //read_filename("bcdefghi",data,512);
    //print_data(data, 0, 64);
    //print_data(data, 256, 512);
    //read_filename("bcdefghi",data,512*4);
    //print_data(data, 0, 64);
    //print_data(data, 512*3, 512*4);
    //write_filename("abcdefgh",count4,512*4);
    //read_filename("abcdefgh",data,512*4);
    //print_data(data, 0, 64);
    //print_data(data, 512*3, 512*4);

    return 0;
}

// Called by system.init. Selects drives, calculates addresses
void disk_init( void ){
    __install_isr(0x06, __diskdriver_06_isr);
    __install_isr(0x0d, __diskdriver_0d_isr);
    __install_isr(0x2e, __diskdriver_2e_isr);

    c_printf( "Starting disk driver\n" );
    __outb(BASE+DEVICE, 0xe0);
    __delay(100); // Wait for device to select
    __outb(CONTROL, 0x02); //Disable interrupts

    //uint8_t data[256];
    //identify( data );
    //print_identify( data );

    uint8_t mbr[512];
    read_sector( mbr, 0 );
    //print_data(mbr, 256, 512);
    //c_getchar();

    uint8_t *part4 = mbr+0x01ee;
    //c_printf("Part 4: ");
    //for (int i=0; i<16; i++) c_printf("%02x ",part4[i]);
    //c_printf("\n");
    part4_lba = *((uint32_t*)(part4+8));

    c_printf("Done disk driver init\n");
}

// Erase all file information
void clear_filetable( void ){
    uint8_t zero[512];
    for (int i=0; i<512; i++) zero[i]=0;
    write_sector(zero, part4_lba);
    c_printf("File table cleared\n");
}

// Gets the index of a given filename
// Returns -1 if file table is full
// Set write to write filename if not found and if space available
int get_filename( char *filename, int write ){
    uint8_t filetable[512];
    read_sector(filetable, part4_lba);

    int i;
    for (i=0; i<64; i++){
        char *s1 = (char *)(filetable+i*8);
        if (strcmp(s1, filename) == 0){
            break;
        }
        if (*s1 == 0){
            if (write){
                _kmemcpy((byte_t *)filetable+i*8, (byte_t *)filename, 8);
                break;
            } else {
                i = 64;
                break;
            }
        }
    }

    if (i==64){
        c_printf("Did not find file \"%s\"\n",filename);
        return -1;
    }

    write_sector(filetable, part4_lba);
    c_printf("Found file \"%s\" at index %d\n",filename,i);
    return i;

}

// Write to the given filename from an array
void write_filename( char *filename, uint8_t data[], uint16_t size ){
    c_printf("Writing %d bytes of %x to file \"%s\"\n",size,data,filename);
    int index = get_filename(filename, 1);
    if (index == -1){
        c_printf("No room in filetable\n");
        return;
    }
    uint32_t lba = part4_lba+1+index*128;
    for (int i=0; i*512<size; i+=1){
        write_sector(data+i*512, lba+i);
        __delay(5);
    }
}

// Read from the given filename into an array
void read_filename( char *filename, uint8_t data[], uint16_t size ){
    c_printf("Reading %d bytes of file \"%s\" to %x\n",size,filename,data);
    int index = get_filename(filename, 0);
    if (index == -1){
        c_printf("File \"%s\" not found in filetable\n", filename);
        return;
    }
    uint32_t lba = part4_lba+1+index*128;
    for (int i=0; i*512<size; i+=1){
        read_sector(data+i*512, lba+i);
        __delay(5);
    }
}

// Print part of a data block
void print_data( uint8_t data[], int start, int end ){
    for (int i=start; i<end; i+=16){
        c_printf("%4x:  %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",i,data[i],data[i+1],data[i+2],data[i+3],data[i+4],data[i+5],data[i+6],data[i+7],data[i+8],data[i+9],data[i+10],data[i+11],data[i+12],data[i+13],data[i+14],data[i+15]);
    }
    c_printf("Press ENTER to continue...");
    c_getchar();
}

// Write a 512 byte array to the given lba
void write_sector( uint8_t data[], uint32_t lba ){
    unsigned int status;
    unsigned int error;

    c_printf("WRITE SECTOR LBA: %08x\n",lba);

    _kmemcpy((byte_t*)disk_buffer, (byte_t*)data, 512);

    status = __inb(BASE+STATUS);
    while (!(status & 0x40)){
        c_printf("WRITE SECTOR not ready yet");
        __delay(5);
        status = __inb(BASE+STATUS);
    }

    //c_printf("WRITE SECTOR pre-setup status: %x\n",status);
    __outb(BASE+SECTOR_COUNT, 0x01);
    __outb(BASE+LBA_LO, lba & 0xff);
    __outb(BASE+LBA_MID, (lba>>8) & 0xff);
    __outb(BASE+LBA_HI, (lba>>16) & 0xff);
    __outb(BASE+DEVICE, (__inb(BASE+DEVICE)&0xf0)|((lba>>24)&0x0f));
    __outb(BASE+COMMAND, 0x30); // WRITE SECTOR command

    status = __inb(BASE+STATUS);
    //c_printf("WRITE SECTOR post-setup status: %x\n",status);

    while (status & 0x80){
        //c_printf("WRITE SECTOR busy status: %x\n",status);
        __delay(1);
        status = __inb(BASE+STATUS);
    }

    while (!(status & 0x08) && !(status & 0x01)){
        c_printf("WRITE SECTOR not-ready status: %x\n",status);
        __delay(5);
        status = __inb(BASE+STATUS);
    }

    if (status & 0x01){
        c_printf("**************************\n"
                 "*** WRITE SECTOR ERROR ***\n"
                 "**************************\n");
        c_printf("WRITE SECTOR ERROR status: %x\n",status);
        error = __inb(BASE+ERROR);
        c_printf("WRITE SECTOR ERROR error:  %x\n",error);
        exit(EXIT_FAILURE);
    }

    //c_printf("WRITE SECTOR ready to read!\n");
    for (int i=0; i<256; i+=1){
        __outw(BASE+DATA, disk_buffer[i]);
    }
    //c_printf("WRITE SECTOR done reading\n");
}

// Read from the given lba to a 512 byte array
void read_sector( uint8_t data[], uint32_t lba ){
    unsigned int status;
    unsigned int error;

    c_printf("READ SECTOR LBA: %08x\n",lba);

    status = __inb(BASE+STATUS);
    while (!(status & 0x40)){
        c_printf("READ SECTOR not ready yet");
        __delay(5);
        status = __inb(BASE+STATUS);
    }

    //c_printf("READ SECTOR pre-setup status: %x\n",status);
    __outb(BASE+SECTOR_COUNT, 0x01);
    __outb(BASE+LBA_LO, lba & 0xff);
    __outb(BASE+LBA_MID, (lba>>8) & 0xff);
    __outb(BASE+LBA_HI, (lba>>16) & 0xff);
    __outb(BASE+DEVICE, (__inb(BASE+DEVICE)&0xf0)|((lba>>24)&0x0f));
    __outb(BASE+COMMAND, 0x20); // READ_SECTOR command

    status = __inb(BASE+STATUS);
    //c_printf("READ SECTOR post-setup status: %x\n",status);

    while (status & 0x80){
        //c_printf("READ SECTOR busy status: %x\n",status);
        __delay(1);
        status = __inb(BASE+STATUS);
    }

    while (!(status & 0x08) && !(status & 0x01)){
        c_printf("READ SECTOR not-ready status: %x\n",status);
        __delay(5);
        status = __inb(BASE+STATUS);
    }

    if (status & 0x01){
        c_printf("*************************\n"
                 "*** READ SECTOR ERROR ***\n"
                 "*************************\n");
        c_printf("READ SECTOR ERROR status: %x\n",status);
        error = __inb(BASE+ERROR);
        c_printf("READ SECTOR ERROR error:  %x\n",error);
        exit(EXIT_FAILURE);
    }

    //c_printf("READ SECTOR ready to read!\n");
    for (int i=0; i<256; i+=1){
        disk_buffer[i] = __inw(BASE+DATA);
    }
    //c_printf("READ SECTOR done reading\n");

    _kmemcpy((byte_t*)data, (byte_t*)disk_buffer, 512);
}

// Run the identify command into a 512 byte array
void identify( uint8_t data[] ){
    unsigned int status;

    c_printf("IDENTIFY\n");

    status = __inb(BASE+STATUS);
    //c_printf("IDENTIFY pre-setup status: %x\n",status);
    __outb(BASE+SECTOR_COUNT, 0x00);
    __outb(BASE+LBA_LO, 0x00);
    __outb(BASE+LBA_MID, 0x00);
    __outb(BASE+LBA_HI, 0x00);
    __outb(BASE+COMMAND, 0xec); // IDENTIFY command

    status = __inb(BASE+STATUS);
    //c_printf("IDENTIFY post-setup status: %x\n",status);

    if (status == 0){
        c_printf("Drive does not exist???\n");
        exit(EXIT_FAILURE);
    }

    while (status & 0x80){
        //c_printf("IDENTIFY busy status: %x\n",status);
        __delay(5);
        status = __inb(BASE+STATUS);
    }

    while (!(status & 0x08) && !(status & 0x01)){
        //c_printf("IDENTIFY not-ready status: %x\n",status);
        __delay(5);
        status = __inb(BASE+STATUS);
    }

    if (status & 0x01){
        c_printf("**********************\n"
                 "*** IDENTIFY ERROR ***\n"
                 "**********************\n");
        c_printf("IDENTIFY ERROR status: %x\n",status);
        exit(EXIT_FAILURE);
    }


    //c_printf("IDENTIFY ready to read!\n");
    for (int i=0; i<256; i+=1){
        disk_buffer[i] = __inw(BASE+DATA);
    }
    //c_printf("IDENTIFY done reading\n");
    _kmemcpy((byte_t*)data, (byte_t*)disk_buffer, 512);
}

// Print some results from the identify command
void print_identify( uint8_t data[] ){
    c_printf("IDENTIFY INFORMATION\n");
    c_printf("    Serial number: %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",
            data[20],data[21],data[22],data[23],
            data[24],data[25],data[26],data[27],
            data[28],data[29],data[30],data[31],
            data[32],data[33],data[34],data[35],
            data[36],data[37],data[38],data[39]);
    c_printf("    Serial number: \"%s\"\n", (char *)(data+20));
}

// Compare two strings. Returns 0 when equal
int strcmp( char *s1, char *s2 ){
    int diff;
    for (int i=0; i<6; i++){
        diff = s1[i]-s2[i];
        if (diff){ return diff; }
        if (s1[i] == 0 || s2[i] == 0){ return diff; }
    }
    return diff;
}

// Interrupts that appeared and should be ignored
void __diskdriver_06_isr( int vector, int code ){
    //c_printf("Unknown 06 interrupt\n");
}
void __diskdriver_0d_isr( int vector, int code ){
    c_printf("Unknown 0d interrupt\n");
}
void __diskdriver_2e_isr( int vector, int code ){
    c_printf("Unknown 2e interrupt\n");
}
