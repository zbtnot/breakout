import Module from '../build/wasm/index';

// setup query selectors
const playBtn = document.querySelector("#play");
const kittenBtn = document.querySelector('#kitten');
const uploadBtn = document.querySelector('#file-picker');
const uploadBtnInput = document.querySelector('#file-picker-input');

// initial state of the web assembly module
let gameModule = null;

// helper function that converts a file blob into an HtmlImageElement
async function blobToImage(blob) {
    const url = URL.createObjectURL(blob);
    const img = new Image();
    const imgBlob = await new Promise(resolve => {
        img.onload = () => resolve(img);
        img.src = url;
    });
    URL.revokeObjectURL(url);

    return imgBlob;
}

// helper function that renders a file blob into an HtmlCanvasElement,
// then returns it as an Int8Array suitable for writing to disk.
async function canvasify(image) {
    const canvas = document.createElement('canvas');
    const context = canvas.getContext('2d');

    // set up our temporary canvas for drawing into it
    canvas.height = 480;
    canvas.width = 640;
    context.drawImage(image, 0, 0, canvas.width, canvas.height);
    const canvasBlob = await new Promise(resolve => canvas.toBlob(resolve, 'image/png', 1));

    return new Int8Array(await canvasBlob.arrayBuffer());
}

// helper method for writing a canvasified-image to disk
function writeImageToDisk(canvasifiedImage)
{
    if (!gameModule) {
        return;
    }

    gameModule.FS.writeFile(
        '/tmp/cat.png',
        canvasifiedImage,
    );

    // inform native code it is time to load the cat!
    gameModule['_loadCat']();
}

// event handler for booting the game
async function play() {
    if (!gameModule) {
        gameModule = await Module({
            canvas: document.querySelector("#canvas"),
        });
        playBtn.setAttribute('disabled', '');
        kittenBtn.removeAttribute('disabled');
        uploadBtn.removeAttribute('disabled');
    }
}

// event handler for downloading cats from the internet
async function kitten() {
    if (!gameModule) {
        return;
    }

    // download the cat as a binary blob
    const catto = await (await fetch('https://placekitten.com/g/640/480')).blob();
    // convert the blob to an HtmlImageElement

    const image = await blobToImage(catto);
    const canvasifiedImage = await canvasify(image);
    writeImageToDisk(canvasifiedImage);
}

// event handler for uploading images from the user's device
async function upload() {
    if (uploadBtnInput.files.length === 0 || !gameModule) {
        return;
    }
    // extract the file and convert it to an img-src
    const file = uploadBtnInput.files[0];
    const imageBlob = await blobToImage(file);
    const canvasIfiedImage = await canvasify(imageBlob);
    writeImageToDisk(canvasIfiedImage);
}

// bind the event handlers
playBtn.addEventListener('click', play);
kittenBtn.addEventListener('click', kitten);
uploadBtn.addEventListener('click', () => uploadBtnInput.click());
uploadBtnInput.addEventListener('change', upload);
