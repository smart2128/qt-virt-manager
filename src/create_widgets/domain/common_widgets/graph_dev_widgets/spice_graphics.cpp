#include "spice_graphics.h"

spice_graphHlpThread::spice_graphHlpThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr) :
    _VirtThread(parent, connPtrPtr)
{
    qRegisterMetaType<QStringList>("QStringList&");
}
void spice_graphHlpThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    QStringList nets;
    virNetworkPtr *networks = nullptr;
    unsigned int flags =
            VIR_CONNECT_LIST_NETWORKS_ACTIVE |
            VIR_CONNECT_LIST_NETWORKS_INACTIVE;
    int ret = virConnectListAllNetworks(
                *ptr_ConnPtr, &networks, flags);
    if ( ret<0 ) {
        sendConnErrors();
    } else {
        // therefore correctly to use for() command,
        // because networks[0] can not exist.
        for (int i = 0; i < ret; i++) {
            nets.append( virNetworkGetName(networks[i]) );
            virNetworkFree(networks[i]);
        };
        if (networks) free(networks);
    };
    //int devs = virNodeNumOfDevices(ptr_ConnPtr, nullptr, 0);
    if ( virConnectClose(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
    };
    emit result(nets);
}

#define KEYMAPs QStringList()<<"auto"<<"en-gb"<<"en-us"<<"ru"<<"fr"<<"de"<<"is"<<"it"<<"ja"

Spice_Graphics::Spice_Graphics(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    _QWidget(parent, connPtrPtr)
{
    addrLabel = new QLabel(tr("Address:"), this);
    address = new QComboBox(this);
    address->setEditable(false);
    address->addItem(tr("HyperVisor default"), "");
    address->addItem(tr("LocalHost only"), "127.0.0.1");
    address->addItem(tr("All Interfaces"), "0.0.0.0");
    address->addItem(tr("Custom"), "custom");
    address->addItem(tr("Use named configured Network"), "network");
    address->addItem(tr("Use Socket"), "socket");
    address->insertSeparator(4);
    address->insertSeparator(6);
    networks = new QComboBox(this);
    networks->setVisible(false);
    autoPort = new QCheckBox(tr("AutoPort"), this);
    port = new QSpinBox(this);
    port->setRange(1000, 65535);
    port->setValue(5900);
    port->setEnabled(false);
    tlsPortLabel = new QCheckBox(tr("Use TLS"), this);
    tlsPort = new QSpinBox(this);
    tlsPort->setRange(10, 65535);
    tlsPort->setValue(5901);
    tlsPort->setEnabled(false);
    usePassw = new QCheckBox(tr("Password"), this);
    passw = new QLineEdit(this);
    passw->setEnabled(false);
    keymapLabel = new QLabel(tr("Keymap"), this);
    keymap = new QComboBox(this);
    keymap->setEditable(true);
    keymap->addItems(KEYMAPs);
    keymap->setEnabled(false);
    defaultLabel = new QLabel(tr("Default policy"), this);
    defaultPolicy = new QComboBox(this);
    defaultPolicy->addItem(tr("Any"), "any");
    defaultPolicy->addItem(tr("Secure"), "secure");
    defaultPolicy->addItem(tr("Insecure"), "insecure");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(addrLabel, 0, 0);
    baseLayout->addWidget(address, 0, 1);
    baseLayout->addWidget(networks, 1, 1);
    baseLayout->addWidget(autoPort, 2, 0);
    baseLayout->addWidget(port, 2, 1);
    baseLayout->addWidget(tlsPortLabel, 3, 0);
    baseLayout->addWidget(tlsPort, 3, 1);
    baseLayout->addWidget(usePassw, 4, 0);
    baseLayout->addWidget(passw, 4, 1);
    baseLayout->addWidget(keymapLabel, 5, 0);
    baseLayout->addWidget(keymap, 5, 1);
    baseLayout->addWidget(defaultLabel, 6, 0);
    baseLayout->addWidget(defaultPolicy, 6, 1);
    baseElements = new QWidget(this);
    baseElements->setLayout(baseLayout);

    mainLabel = new QCheckBox(tr("Main"), this);
    mainLabel->setLayoutDirection(Qt::RightToLeft);
    main = new QComboBox(this);
    main->addItem(tr("Any"), "any");
    main->addItem(tr("Secure"), "secure");
    main->addItem(tr("Insecure"), "insecure");
    main->setEnabled(false);
    displayLabel = new QCheckBox(tr("Display"), this);
    displayLabel->setLayoutDirection(Qt::RightToLeft);
    display = new QComboBox(this);
    display->addItem(tr("Any"), "any");
    display->addItem(tr("Secure"), "secure");
    display->addItem(tr("Insecure"), "insecure");
    display->setEnabled(false);
    inputsLabel = new QCheckBox(tr("Inputs"), this);
    inputsLabel->setLayoutDirection(Qt::RightToLeft);
    inputs = new QComboBox(this);
    inputs->addItem(tr("Any"), "any");
    inputs->addItem(tr("Secure"), "secure");
    inputs->addItem(tr("Insecure"), "insecure");
    inputs->setEnabled(false);
    cursorLabel = new QCheckBox(tr("Cursor"), this);
    cursorLabel->setLayoutDirection(Qt::RightToLeft);
    cursor = new QComboBox(this);
    cursor->addItem(tr("Any"), "any");
    cursor->addItem(tr("Secure"), "secure");
    cursor->addItem(tr("Insecure"), "insecure");
    cursor->setEnabled(false);
    playbackLabel = new QCheckBox(tr("Playback"), this);
    playbackLabel->setLayoutDirection(Qt::RightToLeft);
    playback = new QComboBox(this);
    playback->addItem(tr("Any"), "any");
    playback->addItem(tr("Secure"), "secure");
    playback->addItem(tr("Insecure"), "insecure");
    playback->setEnabled(false);
    recordLabel = new QCheckBox(tr("Record"), this);
    recordLabel->setLayoutDirection(Qt::RightToLeft);
    record = new QComboBox(this);
    record->addItem(tr("Any"), "any");
    record->addItem(tr("Secure"), "secure");
    record->addItem(tr("Insecure"), "insecure");
    record->setEnabled(false);
    smartcardLabel = new QCheckBox(tr("Smartcard"), this);
    smartcardLabel->setLayoutDirection(Qt::RightToLeft);
    smartcard = new QComboBox(this);
    smartcard->addItem(tr("Any"), "any");
    smartcard->addItem(tr("Secure"), "secure");
    smartcard->addItem(tr("Insecure"), "insecure");
    smartcard->setEnabled(false);
    usbredirLabel = new QCheckBox(tr("Usbredir"), this);
    usbredirLabel->setLayoutDirection(Qt::RightToLeft);
    usbredir = new QComboBox(this);
    usbredir->addItem(tr("Any"), "any");
    usbredir->addItem(tr("Secure"), "secure");
    usbredir->addItem(tr("Insecure"), "insecure");
    usbredir->setEnabled(false);
    channelLabel = new QLabel(tr("<b>Channels</b>"), this);
    policyElementsLayout = new QGridLayout();
    policyElementsLayout->addWidget(channelLabel, 0, 0, Qt::AlignRight);
    policyElementsLayout->addWidget(mainLabel, 1, 0);
    policyElementsLayout->addWidget(main, 1, 1);
    policyElementsLayout->addWidget(displayLabel, 2, 0);
    policyElementsLayout->addWidget(display, 2, 1);
    policyElementsLayout->addWidget(inputsLabel, 3, 0);
    policyElementsLayout->addWidget(inputs, 3, 1);
    policyElementsLayout->addWidget(cursorLabel, 4, 0);
    policyElementsLayout->addWidget(cursor, 4, 1);
    policyElementsLayout->addWidget(playbackLabel, 5, 0);
    policyElementsLayout->addWidget(playback, 5, 1);
    policyElementsLayout->addWidget(recordLabel, 6, 0);
    policyElementsLayout->addWidget(record, 6, 1);
    policyElementsLayout->addWidget(smartcardLabel, 7, 0);
    policyElementsLayout->addWidget(smartcard, 7, 1);
    policyElementsLayout->addWidget(usbredirLabel, 8, 0, Qt::AlignTop);
    policyElementsLayout->addWidget(usbredir, 8, 1, Qt::AlignTop);
    policyElements = new QWidget(this);
    policyElements->setVisible(false);
    policyElements->setLayout(policyElementsLayout);

    compress = new QCheckBox(tr("Compression"), this);
    compressImage = new QCheckBox("Image", this);
    compressImage->setLayoutDirection(Qt::RightToLeft);
    compressJpeg = new QCheckBox("Jpeg", this);
    compressJpeg->setLayoutDirection(Qt::RightToLeft);
    compressZlib = new QCheckBox("Zlib", this);
    compressZlib->setLayoutDirection(Qt::RightToLeft);
    compressPlayback = new QCheckBox(tr("Playback"), this);
    compressPlayback->setLayoutDirection(Qt::RightToLeft);
    imageElement = new QComboBox(this);
    imageElement->setEnabled(false);
    imageElement->addItem("auto_glz");
    imageElement->addItem("auto_lz");
    imageElement->addItem("quic");
    imageElement->addItem("glz");
    imageElement->addItem("lz");
    imageElement->addItem("off");
    jpegElement = new QComboBox(this);
    jpegElement->setEnabled(false);
    jpegElement->addItem(tr("Auto"), "auto");
    jpegElement->addItem(tr("Never"), "never");
    jpegElement->addItem(tr("Always"), "always");
    zlibElement = new QComboBox(this);
    zlibElement->setEnabled(false);
    zlibElement->addItem(tr("Auto"), "auto");
    zlibElement->addItem(tr("Never"), "never");
    zlibElement->addItem(tr("Always"), "always");
    playbackElement = new QComboBox(this);
    playbackElement->setEnabled(false);
    playbackElement->addItem(tr("On"), "on");
    playbackElement->addItem(tr("Off"), "off");
    compressElementsLayout = new QGridLayout();
    compressElementsLayout->addWidget(compressImage, 0, 0);
    compressElementsLayout->addWidget(imageElement, 0, 1);
    compressElementsLayout->addWidget(compressJpeg, 1, 0);
    compressElementsLayout->addWidget(jpegElement, 1, 1);
    compressElementsLayout->addWidget(compressZlib, 2, 0);
    compressElementsLayout->addWidget(zlibElement, 2, 1);
    compressElementsLayout->addWidget(compressPlayback, 3, 0);
    compressElementsLayout->addWidget(playbackElement, 3, 1);
    compressElements = new QWidget(this);
    compressElements->setVisible(false);
    compressElements->setLayout(compressElementsLayout);

    addition = new QCheckBox(tr("Addition"), this);
    streaming = new QCheckBox(tr("Streaming"), this);
    streaming->setLayoutDirection(Qt::RightToLeft);
    clipboard = new QCheckBox(tr("Clipboard"), this);
    clipboard->setLayoutDirection(Qt::RightToLeft);
    mouse = new QCheckBox(tr("Mouse"), this);
    mouse->setLayoutDirection(Qt::RightToLeft);
    filetransfer = new QCheckBox(tr("File transfer"), this);
    filetransfer->setLayoutDirection(Qt::RightToLeft);
    streamingElement = new QComboBox(this);
    streamingElement->setEnabled(false);
    streamingElement->addItem("filter");
    streamingElement->addItem("all");
    streamingElement->addItem("off");
    clipboardElement = new QComboBox(this);
    clipboardElement->setEnabled(false);
    //clipboardElement->addItem(tr("Yes), "yes");
    clipboardElement->addItem(tr("No"), "no");
    mouseElement = new QComboBox(this);
    mouseElement->setEnabled(false);
    mouseElement->addItem(tr("Client"), "client");
    mouseElement->addItem(tr("Server"), "server");
    filetransferElement = new QComboBox(this);
    filetransferElement->setEnabled(false);
    //filetransferElement->addItem(tr("Yes), "yes");
    filetransferElement->addItem(tr("No"), "no");
    additionElementsLayout = new QGridLayout();
    additionElementsLayout->addWidget(streaming, 0, 0);
    additionElementsLayout->addWidget(streamingElement, 0, 1);
    additionElementsLayout->addWidget(clipboard, 1, 0);
    additionElementsLayout->addWidget(clipboardElement, 1, 1);
    additionElementsLayout->addWidget(mouse, 2, 0);
    additionElementsLayout->addWidget(mouseElement, 2, 1);
    additionElementsLayout->addWidget(filetransfer, 3, 0);
    additionElementsLayout->addWidget(filetransferElement, 3, 1);
    additionElements = new QWidget(this);
    additionElements->setVisible(false);
    additionElements->setLayout(additionElementsLayout);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseElements);
    commonLayout->addWidget(policyElements);
    commonLayout->addWidget(compress);
    commonLayout->addWidget(compressElements);
    commonLayout->addWidget(addition, -1);
    commonLayout->addWidget(additionElements);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(address, SIGNAL(currentIndexChanged(int)),
            this, SLOT(addressEdit(int)));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(useAutoPort(bool)));
    connect(tlsPortLabel, SIGNAL(toggled(bool)),
            this, SLOT(useTLSPort(bool)));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SLOT(usePassword(bool)));
    autoPort->setChecked(true);
    usePassw->setChecked(false);
    connect(defaultPolicy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(defaultPolicyChanged(int)));
    connect(mainLabel, SIGNAL(toggled(bool)),
            main, SLOT(setEnabled(bool)));
    connect(displayLabel, SIGNAL(toggled(bool)),
            display, SLOT(setEnabled(bool)));
    connect(inputsLabel, SIGNAL(toggled(bool)),
            inputs, SLOT(setEnabled(bool)));
    connect(cursorLabel, SIGNAL(toggled(bool)),
            cursor, SLOT(setEnabled(bool)));
    connect(playbackLabel, SIGNAL(toggled(bool)),
            playback, SLOT(setEnabled(bool)));
    connect(recordLabel, SIGNAL(toggled(bool)),
            record, SLOT(setEnabled(bool)));
    connect(smartcardLabel, SIGNAL(toggled(bool)),
            smartcard, SLOT(setEnabled(bool)));
    connect(usbredirLabel, SIGNAL(toggled(bool)),
            usbredir, SLOT(setEnabled(bool)));
    connect(compress, SIGNAL(toggled(bool)),
            this, SLOT(compressStateChanged(bool)));
    connect(compress, SIGNAL(toggled(bool)),
            this, SLOT(compressStateChanged(bool)));
    connect(compressImage, SIGNAL(toggled(bool)),
            imageElement, SLOT(setEnabled(bool)));
    connect(compressJpeg, SIGNAL(toggled(bool)),
            jpegElement, SLOT(setEnabled(bool)));
    connect(compressZlib, SIGNAL(toggled(bool)),
            zlibElement, SLOT(setEnabled(bool)));
    connect(compressPlayback, SIGNAL(toggled(bool)),
            playbackElement, SLOT(setEnabled(bool)));
    connect(addition, SIGNAL(toggled(bool)),
            this, SLOT(additionStateChanged(bool)));
    connect(streaming, SIGNAL(toggled(bool)),
            streamingElement, SLOT(setEnabled(bool)));
    connect(clipboard, SIGNAL(toggled(bool)),
            clipboardElement, SLOT(setEnabled(bool)));
    connect(mouse, SIGNAL(toggled(bool)),
            mouseElement, SLOT(setEnabled(bool)));
    connect(filetransfer, SIGNAL(toggled(bool)),
            filetransferElement, SLOT(setEnabled(bool)));
    hlpThread = new spice_graphHlpThread(this, connPtrPtr);
    connect(hlpThread, SIGNAL(result(QStringList&)),
            this, SLOT(readNetworkList(QStringList&)));
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(emitCompleteSignal()));
    hlpThread->start();
    // dataChanged connections
    connect(address, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(networks, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(autoPort, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(port, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(tlsPortLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(tlsPort, SIGNAL(valueChanged(int)),
            this, SLOT(stateChanged()));
    connect(usePassw, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(passw, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(keymap, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(defaultPolicy, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(mainLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(displayLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(inputsLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(cursorLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(playbackLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(recordLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(smartcardLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(usbredirLabel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(main, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(display, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(inputs, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(cursor, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(playback, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(record, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(smartcard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(usbredir, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(compress, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(compressImage, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(compressJpeg, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(compressZlib, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(compressPlayback, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(imageElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(jpegElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(zlibElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(playbackElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(addition, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(streaming, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(clipboard, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(mouse, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(filetransfer, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(streamingElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(clipboardElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(mouseElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(filetransferElement, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Spice_Graphics::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _listen, _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "spice");
    _devDesc.setAttribute(
                "defaultPolicy",
                defaultPolicy->currentData(Qt::UserRole).toString());
    if ( tlsPortLabel->isEnabled() && tlsPortLabel->isChecked() ) {
        _devDesc.setAttribute("tlsPort", tlsPort->text());
    };
    if ( autoPort->isEnabled() ) {
        if ( autoPort->isChecked() ) {
            _devDesc.setAttribute("autoport", "yes");
            _devDesc.setAttribute("port", "-1");
        } else {
            _devDesc.setAttribute("port", port->text());
        };
    };
    if ( usePassw->isChecked() ) {
        _devDesc.setAttribute("passwd", passw->text());
        _devDesc.setAttribute("keymap", keymap->currentText());
    };
    if ( policyElements->isVisibleTo(parentWidget()) ) {
        if ( mainLabel->isChecked() ) {
            QDomElement _main = doc.createElement("channel");
            _main.setAttribute("name", "main");
            _main.setAttribute(
                        "mode",
                        main->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_main);
        };
        if ( displayLabel->isChecked() ) {
            QDomElement _display = doc.createElement("channel");
            _display.setAttribute("name", "display");
            _display.setAttribute(
                        "mode",
                        display->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_display);
        };
        if ( inputsLabel->isChecked() ) {
            QDomElement _inputs = doc.createElement("channel");
            _inputs.setAttribute("name", "inputs");
            _inputs.setAttribute(
                        "mode",
                        inputs->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_inputs);
        };
        if ( cursorLabel->isChecked() ) {
            QDomElement _cursor = doc.createElement("channel");
            _cursor.setAttribute("name", "cursor");
            _cursor.setAttribute(
                        "mode",
                        cursor->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_cursor);
        };
        if ( playbackLabel->isChecked() ) {
            QDomElement _playback = doc.createElement("channel");
            _playback.setAttribute("name", "playback");
            _playback.setAttribute(
                        "mode",
                        playback->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_playback);
        };
        if ( recordLabel->isChecked() ) {
            QDomElement _record = doc.createElement("channel");
            _record.setAttribute("name", "record");
            _record.setAttribute(
                        "mode",
                        record->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_record);
        };
        if ( smartcardLabel->isChecked() ) {
            QDomElement _smartcard = doc.createElement("channel");
            _smartcard.setAttribute("name", "smartcard");
            _smartcard.setAttribute(
                        "mode",
                        smartcard->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_smartcard);
        };
        if ( usbredirLabel->isChecked() ) {
            QDomElement _usbredir = doc.createElement("channel");
            _usbredir.setAttribute("name", "usbredir");
            _usbredir.setAttribute(
                        "mode",
                        usbredir->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_usbredir);
        };
    };
    QString _address = address->currentData(Qt::UserRole).toString();
    if ( !_address.isEmpty() && _address!="network" && _address!="socket" ) {
        _listen = doc.createElement("listen");
        _listen.setAttribute("type", "address");
        if ( _address!="custom" ) {
            _listen.setAttribute("address", _address);
            _devDesc.setAttribute("listen", _address);
        } else {
            _listen.setAttribute(
                        "address",
                        address->currentText());
            _devDesc.setAttribute(
                        "listen",
                        address->currentText());
        };
        _devDesc.appendChild(_listen);
    } else if ( _address=="network" && networks->count()>0 ) {
            _listen = doc.createElement("listen");
            _listen.setAttribute("type", "network");
            _listen.setAttribute("network", networks->currentText());
            _devDesc.appendChild(_listen);
    } else if ( _address=="socket" ) {
        _devDesc.setAttribute("socket", address->currentText());
        _listen = doc.createElement("listen");
        _listen.setAttribute("type", "socket");
        _listen.setAttribute("socket", address->currentText());
        _devDesc.appendChild(_listen);
    };
    if ( compress->isChecked() ) {
        if ( compressImage->isChecked() ) {
            QDomElement _image = doc.createElement("image");
            _image.setAttribute("compression", imageElement->currentText());
            _devDesc.appendChild(_image);
        };
        if ( compressJpeg->isChecked() ) {
            QDomElement _jpeg = doc.createElement("jpeg");
            _jpeg.setAttribute(
                        "compression",
                        jpegElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_jpeg);
        };
        if ( compressZlib->isChecked() ) {
            QDomElement _zlib = doc.createElement("zlib");
            _zlib.setAttribute(
                        "compression",
                        zlibElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_zlib);
        };
        if ( compressPlayback->isChecked() ) {
            QDomElement _playback = doc.createElement("playback");
            _playback.setAttribute(
                        "compression",
                        playbackElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_playback);
        };
    };
    if ( addition->isChecked() ) {
        if ( streaming->isChecked() ) {
            QDomElement _streaming = doc.createElement("streaming");
            _streaming.setAttribute(
                        "mode",
                        streamingElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_streaming);
        };
        if ( clipboard->isChecked() ) {
            QDomElement _clipboard = doc.createElement("clipboard");
            _clipboard.setAttribute(
                        "copypaste",
                        clipboardElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_clipboard);
        };
        if ( mouse->isChecked() ) {
            QDomElement _mouse = doc.createElement("mouse");
            _mouse.setAttribute(
                        "mode",
                        mouseElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_mouse);
        };
        if ( filetransfer->isChecked() ) {
            QDomElement _filetransfer = doc.createElement("filetransfer");
            _filetransfer.setAttribute(
                        "enable",
                        filetransferElement->currentData(Qt::UserRole).toString());
            _devDesc.appendChild(_filetransfer);
        };
    };
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Spice_Graphics::setDataDescription(const QString &_xmlDesc)
{
    //qDebug()<<_xmlDesc;
    QDomDocument doc;
    int idx;
    QDomElement _device, _listen,
            _streaming, _clipboard, _mouse, _filetransfer,
            _image, _jpeg, _zlib, _playback, _channel;
    doc.setContent(_xmlDesc);
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    autoPort->setChecked(
                _device.attribute("autoport")=="yes");
    if ( !autoPort->isChecked() ) {
        port->setValue(
                    _device.attribute("port").toInt());
    };
    tlsPortLabel->setChecked( _device.hasAttribute("tlsPort") );
    if ( tlsPortLabel->isChecked() ) {
        tlsPort->setValue(
                    _device.attribute("tlsPort").toInt());
    };
    usePassw->setChecked( _device.hasAttribute("passwd") );
    if ( _device.hasAttribute("passwd") ) {
        QString _password = _device.attribute("passwd");
        passw->setText(_password);
        idx = keymap->findText(
                    _device.attribute("keymap"),
                    Qt::MatchContains);
        keymap->setCurrentIndex( (idx<0)? 0:idx );
    };
    if ( _device.hasAttribute("socket") ) {
        idx = address->findData(
                    "socket", Qt::UserRole, Qt::MatchExactly);
        address->setCurrentIndex( (idx<0)? 3:idx );
        address->setEditText(_device.attribute("socket"));
    } else {
        _listen = _device.firstChildElement("listen");
    };
    if ( !_listen.isNull() ) {
        QString _type, _data;
        _type = _listen.attribute("type");
        _data = _listen.attribute(_type);
        if ( !_type.isEmpty() ) {
            idx = address->findData(
                        _type, Qt::UserRole, Qt::MatchExactly);
            address->setCurrentIndex( (idx<0)? 3:idx );
            if ( _type=="address" ) {
                if ( address->currentIndex()==3 )
                    address->setEditText(_data);
            } else if ( _type=="network" ) {
                idx = networks->findText(
                            _data, Qt::MatchContains);
                networks->setCurrentIndex( (idx<0)? 0:idx );
            } else if ( _type=="socket" ) {
                address->setEditText(_data);
            } else {
                address->setCurrentIndex(0);
            };
        } else {
            address->setCurrentIndex(0);
        };
    };
    QString _defaultPolicy = _device.attribute("defaultPolicy");
    idx = defaultPolicy->findData(
                _defaultPolicy, Qt::UserRole, Qt::MatchExactly);
    defaultPolicy->setCurrentIndex( (idx<0)? 0:idx );
    _channel = _device.firstChildElement("channel");
    while ( !_channel.isNull() ) {
        QComboBox *obj = nullptr;
        QString _name, _mode;
        _name = _channel.attribute("name");
        _mode = _channel.attribute("mode");
        if ( _name=="main" ) {
            mainLabel->setChecked(true);
            obj = main;
        } else if ( _name=="display" ) {
            displayLabel->setChecked(true);
            obj = display;
        } else if ( _name=="inputs" ) {
            inputsLabel->setChecked(true);
            obj = inputs;
        } else if ( _name=="cursor" ) {
            cursorLabel->setChecked(true);
            obj = cursor;
        } else if ( _name=="playback" ) {
            playbackLabel->setChecked(true);
            obj = playback;
        } else if ( _name=="record" ) {
            recordLabel->setChecked(true);
            obj = record;
        } else if ( _name=="smartcard" ) {
            smartcardLabel->setChecked(true);
            obj = smartcard;
        } else if ( _name=="usbredir" ) {
            usbredirLabel->setChecked(true);
            obj = usbredir;
        };
        if ( nullptr!=obj ) {
            idx = obj->findData(_mode, Qt::UserRole, Qt::MatchExactly);
            obj->setCurrentIndex( (idx<0)? 0:idx);
        };
        _channel = _channel.nextSiblingElement("channel");
    };
    _streaming = _device.firstChildElement("streaming");
    _clipboard = _device.firstChildElement("clipboard");
    _mouse = _device.firstChildElement("mouse");
    _filetransfer = _device.firstChildElement("filetransfer"),
    _image = _device.firstChildElement("image");
    _jpeg = _device.firstChildElement("jpeg");
    _zlib = _device.firstChildElement("zlib");
    _playback = _device.firstChildElement("playback ");
    streaming->setChecked( !_streaming.isNull() );
    clipboard->setChecked( !_clipboard.isNull() );
    mouse->setChecked( !_mouse.isNull() );
    filetransfer->setChecked( !_filetransfer.isNull() );
    compressImage->setChecked( !_image.isNull() );
    compressJpeg->setChecked( !_jpeg.isNull() );
    compressZlib->setChecked( !_zlib.isNull() );
    compressPlayback->setChecked( !_playback.isNull() );
    if ( !_streaming.isNull() ||
         !_clipboard.isNull() ||
         !_mouse.isNull()     ||
         !_filetransfer.isNull() ) {
        addition->setChecked(true);
        if ( !_streaming.isNull() ) {
            idx = streamingElement->findData(
                        _streaming.attribute("mode"));
            streamingElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_clipboard.isNull() ) {
            idx = clipboardElement->findData(
                        _clipboard.attribute("copypaste"),
                        Qt::UserRole, Qt::MatchExactly);
            clipboardElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_mouse.isNull() ) {
            idx = mouseElement->findData(
                        _mouse.attribute("mode"),
                        Qt::UserRole, Qt::MatchExactly);
            mouseElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_filetransfer.isNull() ) {
            idx = filetransferElement->findData(
                        _filetransfer.attribute("enable"),
                        Qt::UserRole, Qt::MatchExactly);
            filetransferElement->setCurrentIndex( (idx<0)? 0:idx );
        }
    } else {
        addition->setChecked(false);
    };
    if ( !_image.isNull() ||
         !_jpeg.isNull()  ||
         !_zlib.isNull()  ||
         !_playback.isNull() ) {
        compress->setChecked(true);
        if ( !_image.isNull() ) {
            idx = imageElement->findText(
                        _image.attribute("compression"));
            imageElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_jpeg.isNull() ) {
            idx = jpegElement->findData(
                        _jpeg.attribute("compression"),
                        Qt::UserRole, Qt::MatchExactly);
            jpegElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_zlib.isNull() ) {
            idx = zlibElement->findData(
                        _zlib.attribute("compression"),
                        Qt::UserRole, Qt::MatchExactly);
            zlibElement->setCurrentIndex( (idx<0)? 0:idx );
        };
        if ( !_playback.isNull() ) {
            idx = playbackElement->findData(
                        _playback.attribute("compression"),
                        Qt::UserRole, Qt::MatchExactly);
            playbackElement->setCurrentIndex( (idx<0)? 0:idx );
        }
    } else {
        compress->setChecked(false);
    };
}

/* private slots */
void Spice_Graphics::useAutoPort(bool state)
{
    port->setEnabled(!state);
    policyElementsSetRequired( !state && tlsPortLabel->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::useTLSPort(bool state)
{
    tlsPort->setEnabled(state);
    policyElementsSetRequired( state && !autoPort->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::usePassword(bool state)
{
    passw->setEnabled(state);
    keymap->setEnabled(state);
}
void Spice_Graphics::addressEdit(int i)
{
    QString s = address->itemData(i, Qt::UserRole).toString();
    if ( s == "network" ) {
        address->setEditable(false);
        addrLabel->setText(tr("Network:"));
        networks->setVisible(true);
        autoPort->setEnabled(true);
        port->setEnabled(true);
        tlsPortLabel->setEnabled(true);
        tlsPort->setEnabled(true);
    } else if ( s == "socket" ) {
        addrLabel->setText(tr("Socket:"));
        address->setEditable(true);
        address->clearEditText();
        networks->setVisible(false);
        autoPort->setEnabled(false);
        port->setEnabled(false);
        tlsPortLabel->setEnabled(false);
        tlsPort->setEnabled(false);
    } else {
        addrLabel->setText(tr("Address:"));
        if ( s == "custom" ) {
            address->setEditable(true);
            address->clearEditText();
        } else {
            address->setEditable(false);
        };
        networks->setVisible(false);
        autoPort->setEnabled(true);
        port->setEnabled(true);
        tlsPortLabel->setEnabled(true);
        tlsPort->setEnabled(true);
    }
}
void Spice_Graphics::policyElementsSetRequired(bool state)
{
    policyElements->setVisible(state);
}
void Spice_Graphics::defaultPolicyChanged(int i)
{
    if ( !policyElements->isVisible() ) {
        main->setCurrentIndex(i);
        display->setCurrentIndex(i);
        inputs->setCurrentIndex(i);
        cursor->setCurrentIndex(i);
        playback->setCurrentIndex(i);
        record->setCurrentIndex(i);
        smartcard->setCurrentIndex(i);
        usbredir->setCurrentIndex(i);
    };
    policyElementsSetRequired( tlsPortLabel->isChecked()
                         && !autoPort->isChecked()
                         && defaultPolicy->currentIndex()>0 );
}
void Spice_Graphics::compressStateChanged(bool state)
{
    compressElements->setVisible(state);
}
void Spice_Graphics::additionStateChanged(bool state)
{
    additionElements->setVisible(state);
}
void Spice_Graphics::readNetworkList(QStringList &_nets)
{
    nets = _nets;
    networks->addItems(nets);
}
void Spice_Graphics::emitCompleteSignal()
{
    if ( sender()==hlpThread ) {
        setEnabled(true);
        emit complete();
    }
}
