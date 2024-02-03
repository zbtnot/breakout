import Module from '../build/wasm/index';

const playBtn = document.querySelector("#play");
const kittenBtn = document.querySelector('#kitten');
let gameModule = null;

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

async function play() {
    if (!gameModule) {
        gameModule = await Module({
            canvas: document.querySelector("#canvas"),
        });
        playBtn.setAttribute('disabled', '');
        kittenBtn.removeAttribute('disabled');
    }
}

async function kitten() {
    if (!gameModule) {
        return;
    }

    // download the cat as a binary blob
    const catto = await (await fetch('https://placekitten.com/g/640/480')).blob();

    const canvas = document.createElement('canvas');
    const context = canvas.getContext('2d');
    
    // convert the blob to an img-src
    const cattoImg = await blobToImage(catto);

    // set up our temporary canvas for drawing into it
    canvas.height = cattoImg.naturalHeight;
    canvas.width = cattoImg.naturalWidth;
    context.drawImage(cattoImg, 0, 0, cattoImg.naturalWidth, cattoImg.naturalHeight);
    const canvasBlob = await new Promise(resolve => canvas.toBlob(resolve, 'image/png', 1));
    const bufferView = new Int8Array(await canvasBlob.arrayBuffer());
    
    gameModule.FS.writeFile(
        '/tmp/cat.png',
        bufferView
    );
    
    // inform native code it is time to load the cat!
    gameModule['_loadCat']();
}

playBtn.addEventListener("click", play);
kittenBtn.addEventListener('click', kitten);