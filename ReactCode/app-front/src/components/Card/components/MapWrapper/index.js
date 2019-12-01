import React from 'react'
import GoogleMapReact from 'google-map-react'

import mapPin from '../../../../assets/map-pin.svg'
import './index.less'

const AnyReactComponent = () => <div className="pin__wrapper"><img src={mapPin} alt="mapPin" /></div>

class MapWrapper extends React.Component {
 
  render() {
    return (
      // Important! Always set the container height explicitly
      <div style={{ height: '30vh', width: '70%' }}>
        <GoogleMapReact
          bootstrapURLKeys={{ key: 'AIzaSyCfX0_CrDjekaD2QgKpdH44jnjr1Xvb43U' }}
          defaultCenter={{
            lat: 0,
            lng: 0
          }}
          center={{
            lat: this.props.coord.lat,
            lng: this.props.coord.lng
          }}
          defaultZoom={13}
        >
          <AnyReactComponent
            lat={this.props.coord.lat}
            lng={this.props.coord.lng}
          />
        </GoogleMapReact>
      </div>
    );
  }
}

export default MapWrapper
