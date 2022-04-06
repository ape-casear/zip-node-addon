declare namespace ZipNodeAddon {
    /**
     * unzip from buffer
     * @param buf 
     * @param destDir 
     * @return json string for entries
     */
    function unzipStream(buf: Buffer, destDir: string): Promise<String>;
    /**
     * unzip from zip file
     * @param zipFile 
     * @param destDir 
     * @return json string for entries
     */
    function unzipFile(zipFile: string, destDir: string): Promise<String>;

    function listAllEntriesFromFile(zipFile: string): Promise<String>;
    function listAllEntriesFromBuffer(buf: Buffer): Promise<String>;
    /**
     * zip files
     * @param param0 
     */
    function zipBuffer({ [key]: Buffer }): Promise<Uint8Array>;
}

declare var ZipNodeAddon: ZipNodeAddon;

export = ZipNodeAddon;