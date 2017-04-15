class DataInputStream {

    constructor(buffer) {
        this.b = new Uint8Array(buffer);
        this.p = 0;
    }

    read(size) {
        if (size) {
            let arr = new Array(size);
            for(let i = 0; i < size; i++) {
                arr[i] = this.read();
            }
            return arr;
        } else {
            return this.b[this.p++];
        }
    }

    readByte() {
        let b1 = this.read();
        let sign = b1 & (1 << 7);
        let i = b1 & 0x7F;
        if (sign) {
            i = i - 0x80;
        }
        return i;
    }

    readUnsignedByte() {
        return this.read();
    }

    readShort() {
        let b1 = this.read();
        let b2 = this.read();
        let sign = b1 & (1 << 7);
        let i = ((b1 & 0x7F) << 8) | b2;
        if (sign) {
            i = i - 0x8000;
        }
        return i;
    }

    readUnsignedShort() {
        let b1 = this.read();
        let b2 = this.read();
        return (b1 << 8) | b2;
    }

    readInt() {
        let b1 = this.read();
        let b2 = this.read();
        let b3 = this.read();
        let b4 = this.read();
        return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
    }

    // readLong() {
    //     let mask5 = (0xFF << 23) * 2; //0x0000_0000_FF00_0000
    // }

    readChar() {
        let b1 = this.read();
        let b2 = this.read();
        return String.fromCharCode((b1 << 8) | b2);
    }

    readString() {
        let size = this.readUnsignedShort();
        return ArrayToUtf8(this.read(size));
    }

}

function ArrayToUtf8(array) {
    var out, i, len, c;
    var char2, char3;

    out = "";
    len = array.length;
    i = 0;
    while(i < len) {
        c = array[i++];
        switch(c >> 4)
        {
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            // 0xxxxxxx
            out += String.fromCharCode(c);
            break;
            case 12: case 13:
            // 110x xxxx   10xx xxxx
            char2 = array[i++];
            out += String.fromCharCode(((c & 0x1F) << 6) | (char2 & 0x3F));
            break;
            case 14:
                // 1110 xxxx  10xx xxxx  10xx xxxx
                char2 = array[i++];
                char3 = array[i++];
                out += String.fromCharCode(((c & 0x0F) << 12) |
                    ((char2 & 0x3F) << 6) |
                    ((char3 & 0x3F) << 0));
                break;
        }
    }

    return out;
}